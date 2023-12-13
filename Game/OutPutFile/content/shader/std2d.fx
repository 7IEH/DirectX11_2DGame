#ifndef _STD2D
#define _STD2D

SamplerState samplerType;

Texture2D shaderTexture;

cbuffer Worldspcae : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
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
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 WorldPos = mul(float4(_in.vPos, 1.f), World);
    float4 ViewPos;
    float4 ProjectionPos;
    
    output.vPosition = float4(WorldPos.xyzw);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    //return float4(0.f, 0.f, 1.f, 1.f);
    
    float4 color = shaderTexture.Sample(samplerType, _in.vUV);
    return color;
}

#endif