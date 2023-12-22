#ifndef _STD2D
#define _STD2D

SamplerState samplerType;

Texture2D shaderTexture;

// Luna Light Example
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
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
    DirectionalLight gDirLight;
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
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float3 vNomral : NORMAL;
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
    
    //float4 WorldPos = mul(float4(_in.vPos, 1.f), World);
    //float4 ViewPos = mul(WorldPos, View);
    //float4 ProjectionPos = mul(ViewPos, Projection);
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    //output.vNomral = mul(vNormal, (float3x3)World);
       
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    _in.vNomral = (0.f, 0.f, -1.f);
    
    float3 toEye = float3(0.f, 0.f, 1.f);
    
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 spec = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 A, D, S;
    
    ComputeDirectionalLight(gMatrial, gDirLight, _in.vNomral, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
      
    float4 lightColor = ambient + diffuse + spec;
    
    float4 color = shaderTexture.Sample(samplerType, _in.vUV) + lightColor;
    //color.a = diffuse.a;
    return color;
}
#endif