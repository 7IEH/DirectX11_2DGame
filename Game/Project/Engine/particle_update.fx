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
#define CenterPos   gMatrial._LightMat.Ambient.xyz

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
                Particle._NoiseForce = (float3) 0.f;
                Particle._NoiseForceTime = 0.f;
                
                float2 vUv = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUv.x += gAccTime * 0.2f;
                
                vUv.y += sin(vUv.x * 20.f * 3.14f) * 0.2f + gAccTime * 0.1f;
                
                float4 vRand = g_NoiseTex.SampleLevel(samplerType2, vUv, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(samplerType2, vUv - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(samplerType2, vUv - float2(0.2f, 0.2f), 0);
                
                if (0 == Module.spawnShape)
                {
                    float RandomRadius = vRand.x * Module.Radius;
                    float RandomAngle = vRand * 2 * 3.14f;
                    
                    Particle._LocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
                }
                else if (1 == Module.spawnShape)
                {
                    Particle._LocalPos.x = vRand[0] * Module._spawnBoxScale.x - (Module._spawnBoxScale.x / 2.f);
                    Particle._LocalPos.y = vRand[1] * Module._spawnBoxScale.y - (Module._spawnBoxScale.y / 2.f);
                    Particle._LocalPos.z = 0.f;
                }
                
                Particle._WorldPos.xyz = Particle._LocalPos.xyz + CenterPos;
    
                // 스폰 크기 설정
                // 최대 크기 - 최소 크기 * (0~1)까지의 랜덤값 즉, (0~(최대크기-최소크기)) -> (최대 크기 ~ 최소 크기) 까지의 영역이 나옴
 
                Particle._WorldInitScale = Particle._WorldScale = (Module._vSpawnMaxScale - Module._vSpawnMinScale) * vRand[2] + Module._vSpawnMinScale;
               
                // 스폰 Life 설정
                Particle._Age = 0.f;
                Particle._Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MinLife;
                
                Particle._Mass = clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                
                // Add VelocityModule
                if (Module._arrModuleCheck[3])
                {
                    // 0 : From Center
                    if (0 == Module._AddVelocityType)
                    {
                        float3 vDir = normalize(Particle._LocalPos.xyz);
                        Particle._Velocity.xyz = vDir * clamp(vRand[2], Module._MinSpeed, Module._MaxSpeed);
                    }
                    if (1 == Module._AddVelocityType)
                    {
                        float3 vDir = -normalize(Particle._LocalPos.xyz);
                        Particle._Velocity.xyz = vDir * clamp(vRand[2], Module._MinSpeed, Module._MaxSpeed);
                    }
                }
                else
                {
                    Particle._Velocity.xyz = float3(0.f, 0.f, 0.f);
                }
                
                
                if (Module._arrModuleCheck[6])
                {
                    // 색깔 다체롭게
                    if (1 == Module._ColorType)
                    {
                        Particle._Color = float4(Module._vSpawnColor.xyz * (30.f - 20.f) * vRand[2], 1.f);
                    }
                }
                else
                {
                    Particle._Color = Module._vSpawnColor;
                }
                
                break;
            }
        }
    }
    
    // 파티클이 활성화 상태라면
    else
    {
       Particle._Age += gDt;
        if (Particle._Life < Particle._Age)
        {
            if (0 == Module._iLoop)
            {
                Particle._Active = 0;
                return;
            }
        }
        
        // 랜덤값 추출
        float2 vUv = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        vUv.x += gAccTime * 0.2f;
        vUv.y = sin(vUv.x * 20.f * 3.14f) * 0.2f + gAccTime * 0.1f;
        float4 vRand = g_NoiseTex.SampleLevel(samplerType2, vUv, 0);
        
        Particle._Force.xyz = float3(0.f, 0.f, 0.f);
        
        Particle._NomarlizedAge = Particle._Age / Particle._Life;
        
        // Scale Module
        if (Module._arrModuleCheck[2])
        {
            Particle._WorldScale = Particle._WorldInitScale * (1.f + (Module._vScaleRatio - 1.f) * Particle._NomarlizedAge);
        }
        
        if (Module._arrModuleCheck[4])
        {
            if (Particle._NoiseForceTime == 0.f)
            {
                Particle._NoiseForce = (normalize(vRand.xyz - 0.5f) + 0.2f) * Module._noiseForceScale;
                Particle._NoiseForceTime = gAccTime;
            }
            else if (Module.NoiseForceTerm < gAccTime - Particle._NoiseForceTime)
            {
                Particle._NoiseForce = (normalize(vRand.xyz - 0.5f) + 0.2f) * Module._noiseForceScale;
                Particle._NoiseForceTime = gAccTime;
            }
        }
        
        if (Module._arrModuleCheck[5])
        {
            Particle._Force.xyz += Particle._NoiseForce.xyz;
            
            float3 vAccel = Particle._Force.xyz / Particle._Mass;
            
            Particle._Velocity.xyz += vAccel * gDt;
        }
        
        if (Module._arrModuleCheck[6])
        {
            if (0 == Module._ColorType)
            {
                 // 색깔 점점 어두워짐
                if (0 == Particle._iFadeVariable)
                {
                    Particle._Color = Module._vSpawnColor * (1.f + (Module._vScaleRatio - 1.f) * Particle._NomarlizedAge);
                    Particle._Color.a = 1.f;
                    if (1.f <= Particle._NomarlizedAge)
                    {
                        Particle._Age = 0.f;
                        Particle._iFadeVariable = 1;
                    }
                }
                 // 색깔 점점 밝아짐
                else
                {
                    Particle._Color = Module._vSpawnColor * Particle._NomarlizedAge;
                    Particle._Color.a = 1.f;
                    if (1.f <= Particle._NomarlizedAge)
                    {
                        Particle._Age = 0.f;
                        Particle._iFadeVariable = 0;
                    }
                }
            }
        }
        
        if (0 == Module.SpaceType)
        {
            // local pos는 잔상이 남음
            // 이유는 localpos는 말 그대로 파티클 하나 하나가 개인 포지션을 가지는 것이므로
            Particle._LocalPos.xyz += Particle._Velocity.xyz * gDt;
            Particle._WorldPos.xyz = Particle._LocalPos.xyz + CenterPos;
        }
        else if (1 == Module.SpaceType)
        {
            // worldpos는 파티클 하나가 해당 파티클 오브젝트를 딱 따라감
            // 잔상이 남을 수 없음
            Particle._WorldPos.xyz += Particle._Velocity.xyz * gDt;
        }
    }
}

#endif