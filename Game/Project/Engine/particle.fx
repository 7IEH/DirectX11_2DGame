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
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.InstID = _in.iInstID;
    
    return output;
}

[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };

    // GS 가 담당하는 파티클 정보를 가져온다.
    tParticle particle = g_ParticleBuffer[(int) _in[0].InstID];
    
    if (0 == particle._Active)
    {
        return;
    }
    
    // 파티클의 중심점을 월드로 이동    
    float3 vWorldPos = particle._WorldPos.xyz;
    
    // 파티클의 중심점을 ViewSpace 로 이동
    float4 vViewPos = mul(View, float4(vWorldPos, 1.f));
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    output[0].vPosition = vViewPos + float4((particle._WorldScale.x * -0.5f), (particle._WorldScale.y * 0.5f), 0.f, 0.f);
    output[1].vPosition = vViewPos + float4((particle._WorldScale.x * 0.5f), (particle._WorldScale.y * 0.5f), 0.f, 0.f);
    output[2].vPosition = vViewPos + float4((particle._WorldScale.x * 0.5f), (particle._WorldScale.y * -0.5f), 0.f, 0.f);
    output[3].vPosition = vViewPos + float4((particle._WorldScale.x * -0.5f), (particle._WorldScale.y * -0.5f), 0.f, 0.f);
    
    output[0].vPosition = mul(Projection,output[0].vPosition);
    output[1].vPosition = mul(Projection,output[1].vPosition);
    output[2].vPosition = mul(Projection,output[2].vPosition);
    output[3].vPosition = mul(Projection,output[3].vPosition);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (gMatrial.spriteCheck0)
    {
        vOutColor = ST0.Sample(samplerType2, _in.vUV) * g_ParticleBuffer[(uint) _in.InstID]._Color;
    }
    else
    {
        vOutColor = g_ParticleBuffer[(uint) _in.InstID]._Color;
    }
    
    return vOutColor;
}


#endif