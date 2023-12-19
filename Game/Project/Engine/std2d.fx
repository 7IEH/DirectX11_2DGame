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
    matrix View;
    matrix Projection;
};

cbuffer Material : register(b1)
{
    Material gMatrial;
}

cbuffer Light : register(b2)
{
    DirectionalLight gDirLight;
    float3 gEyePosW;
    float  pad;
};

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
    float2 vUV : TEXCOORD;
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

//technique11 ColorTech
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, VS()));
//        SetVertexShader(CompileShader(ps_5_0, PS()));

//        setRasterizerstate(WireframeRS);
//    }
//};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 WorldPos = mul(float4(_in.vPos, 1.f), World);
    float4 ViewPos = mul(WorldPos, View);
    float4 ProjectionPos = mul(ViewPos, Projection);
    
    output.vPosition = ProjectionPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 color = shaderTexture.Sample(samplerType, _in.vUV);
    return color;
}

#endif