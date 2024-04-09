#ifndef _PLAYERVS
#define _PLAYERVS

#include "struct.fx"
#include "func.fx"

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
    float3 vWorld : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    float4 _world = mul(float4(_in.vPos.x, _in.vPos.y, _in.vPos.z, 1.f), World);
    output.vWorld = _world.xyz;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}

#endif