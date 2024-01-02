#ifndef _DEBUG2D
#define _DEBUG2D

#include "struct.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_DebugShader(VS_IN _in)
{
    VS_OUT _out = (VS_OUT)0.f;
    
    _out.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    _out.vUV = _in.vUV;
   
    return _out;
}

float4 PS_DebugShader(VS_OUT _in) : SV_Target
{
    float4 color = (float) 0.f;
    
    color = gMatrial._LightMat.Ambient;
    
    return color;
}

#endif