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

    // ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (0 == Particle._Active)
    {
        if (!Module._arrModuleCheck[0])
            return;
        
        while (0 < SpawnCount)
        {
            // Atomic �Լ� 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
                        
            /*interlockedcompareexchange(����, ���� ���, ���°�, origin)
            ������ ���� ����� ������ ������ ���°����� �����ϰ�
            ������ origin���� �ش� ���� ����*/
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
    
    // ��ƼŬ�� Ȱ��ȭ ���¶��
    else
    {
        //Particle._WorldPos.y += gDt * 200.f;
    }
}

#endif