#ifndef _PARTICLEUPDTAE
#define _PARTICLEUPDATE

#include "struct.fx"

RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_SpawnCount : register(u1);
StructuredBuffer<tParticleModule> g_Module : register(t20);

#define MAX_COUNT   gMatrial._int0
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;

    // 파티클이 비활성화 상태라면
    if (0 == Particle._Active)
    {
        if (!Module._arrModuleCheck[0])
            return;
        
        while (0 < SpawnCount)
        {
            // Atomic 함수 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
                        
            /*interlockedcompareexchange(원본, 비교할 대상, 교쳇값, origin)
            원본과 비교할 대상이 같으면 원본을 교쳇값으로 변경하고
            성공시 origin에는 해당 원본 복사*/
            InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            if (AliveCount == Origin)
            {
                Particle._Active = 1;
                
                float2 vUv = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                float4 vColor = g_NoiseTex.SampleLevel(samplerType2, vUv, 0);
                
                if(0==Module.spawnShape)
                {
                    float RandomRadius = vColor.x * Module.Radius;
                    
                    Particle._WorldPos = float4(RandomRadius, RandomRadius, Particle._WorldPos.z, Particle._WorldPos.w);
                    
                    Module.SpaceType;
                }
                
                break;
            }
        }
    }
    
    // 파티클이 활성화 상태라면
    else
    {
        //Particle._WorldPos.y += gDt * 200.f;
    }
}

#endif