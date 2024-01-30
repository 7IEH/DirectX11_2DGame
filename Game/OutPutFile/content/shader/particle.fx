#ifndef _PARTICLE
#define _PARTICLE

#include "struct.fx"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);

#define Particle g_ParticleBuffer[_in.iInstID]

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vWorldPos = (_in.vPos * Particle._WorldScale.xyz) + Particle._WorldPos.xyz;
    
    output.vPosition = mul(Projection, mul(View, float4(vWorldPos, 1.f)));
    output.vUV = _in.vUV;
    output.InstID = _in.iInstID;
    
    return output;
}

float4 PS_Particle(VS_OUT _in) : SV_Target
{
    if (!g_ParticleBuffer[(uint) _in.InstID]._Active)
    {
        discard;
    }
    
    return g_ParticleBuffer[(uint) _in.InstID]._Color;
}


#endif