#ifndef _STD2D
#define _STD2D

#include "struct.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    //float3 vNomral : NORMAL;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float3 vWorld : POSITION;
    float2 vUV : TEXCOORD;
};

// Luna Light Example
void ComputeDirectionalLight(LightMaterial mat, DirectionalLight L,
                             float3 normal, float3 toEye,
                             out float4 ambient,
                             out float4 diffuse,
                             out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.f);
    
    float3 lightVec = -L.Direction;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if(diffuseFactor>0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

void ComputePointLight(LightMaterial mat, PointLight L, float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.Position - pos;
    
    float d = length(lightVec);
    
    if(d>L.Range)
        return;
    
    lightVec /= d;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if(diffuseFactor>0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
    
    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(LightMaterial mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.Position - pos;
    
    float d = length(lightVec);
    
    if (d > L.Range)
        return;
    
    lightVec /= d;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
    
    float att = spot / dot(L.Att, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

RasterizerState WireframeRS
{
    FillMode = Wireframe;
    CullMode = BACK;
    FrontCounterClockwise = false;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_TEST _v;
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vWorld = mul(float4(_in.vPos, 1.f), World).xyz;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
         
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float3 toEye = float3(0.f, 0.f, -0.1f);
    
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 spec = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 A, D, S;
    
    //ComputeDirectionalLight(gMatrial, gLight._DL, vNormal, toEye, A, D, S);
    //ambient += A;
    //diffuse += D;
    //spec += S;
    
    //ComputePointLight(gMatrial, gLight._PL, _in.vWorld, vNormal, toEye, A, D, S);
    //ambient += A;
    //diffuse += D;
    //spec += S;
      
    ComputeSpotLight(gMatrial._LightMat, gLight._SL, _in.vWorld, vNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    float4 lightColor = ambient + diffuse + spec;
    float4 color = (float4) 0.f;
    
    if(gAnimUse == 1)
    {
        float2 animUv = _in.vUV * gSliceSize + gLeftTop + gOffsetSize;
        color = atlas_texture.Sample(samplerType2, animUv) + lightColor;
        
        float2 vBackgroundLeftTop = gLeftTop + (gSliceSize / 2.f) - (gBackground / 2.f);
        vBackgroundLeftTop -= gOffsetSize;
        float2 vUV = vBackgroundLeftTop + (gBackground * _in.vUV);
        
        if (vUV.x < gLeftTop.x || (gLeftTop.x + gSliceSize.x) < vUV.x
            || vUV.y < gLeftTop.y || (gLeftTop.y + gSliceSize.y) < vUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            color = atlas_texture.Sample(samplerType2, vUV);
        }
        
    }
    else
    {
        if (gMatrial.spriteCheck0 == 1)
        {
            color = ST0.Sample(samplerType2, _in.vUV) + lightColor;
        //color.a = diffuse.a;
        }
    }
       
    return color;
}
#endif