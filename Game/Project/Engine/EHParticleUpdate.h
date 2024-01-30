#pragma once

#include "EHComputeShader.h"

class ParticleUpdate
	:public ComputeShader
{
private:
	StructuredBuffer* m_ParticleBuffer;
	StructuredBuffer* m_ParticleModuleBuffer;
	StructuredBuffer* m_SpawnCountBuffer;

public:
	void SetParticleBuffer(StructuredBuffer* _particleBuffer) { m_ParticleBuffer = _particleBuffer; }
	void SetParticleModuleBuffer(StructuredBuffer* _Buffer) { m_ParticleModuleBuffer = _Buffer; }
	void SetParticleSpawnCount(StructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }

public:
	virtual HRESULT UpdateData()override;
	virtual void UpdateGroupCount()override;
	virtual void Clear() override;

public:
	ParticleUpdate();
	virtual ~ParticleUpdate();
};

