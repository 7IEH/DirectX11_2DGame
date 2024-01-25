#ifndef _PARTICLE
#define _PARTICLE

#include "struct.fx"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);

#define Particle g_ParticleBuffer[gMatrial._int0]

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

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
   Particle._WorldPos;
    
    float3 vWorldPos = (_in.vPos * Particle._WorldScale.xyz) + Particle._WorldPos.xyz;
    
    output.vPosition = mul(Projection, mul(View, float4(vWorldPos, 1.f)));
 
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Particle(VS_OUT _in) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}


#endif