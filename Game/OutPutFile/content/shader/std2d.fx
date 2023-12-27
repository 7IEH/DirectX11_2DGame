#ifndef _STD2D
#define _STD2D

Texture2D shaderTexture : register(t0);

SamplerState samplerType : register(s0);
SamplerState samplerType2 : register(s1);

// Luna Light Example
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Direction;
    float Spot;
    float3 Att;
    float pad;
};

struct tLight
{
    DirectionalLight _DL;
    PointLight _PL;
    SpotLight _SL;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Reflect;
};

cbuffer Worldspcae : register(b0)
{
    matrix World;
    matrix matWorldInv;
    
    matrix View;
    matrix matViewInv;
    
    matrix Projection;
    matrix matProjInv;
    
    matrix WV;
    matrix WVP;
};

cbuffer Material : register(b1)
{
    Material gMatrial;
}

cbuffer Light : register(b2)
{
    tLight gLight;
};

cbuffer Normal : register(b3)
{
    float3 vNormal;
    float pad1;
}

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

struct VS_TEST
{
    float4 t_ambient;
    float4 t_diffuse;
    float4 t_spec;
};

// Luna Light Example
void ComputeDirectionalLight(Material mat, DirectionalLight L,
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

void ComputePointLight(Material mat, PointLight L,float3 pos,float3 normal,float3 toEye,
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

void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
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
      
    ComputeSpotLight(gMatrial, gLight._SL, _in.vWorld, vNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    float4 lightColor = ambient + diffuse + spec;
    
    float4 color = shaderTexture.Sample(samplerType2, _in.vUV) + lightColor;
    //color.a = diffuse.a;
    return color;
}
#endif