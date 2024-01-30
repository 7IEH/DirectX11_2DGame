#pragma once

#include "EHComputeShader.h"

class ParticleUpdate
	:public ComputeShader
{
private:
	StructuredBuffer* m_ParticleBuffer;
	StructuredBuffer* m_ParticleModuleBuffer;
	StructuredBuffer* m_SpawnCountBuffer;
	Vec3			  m_ParticleWorldPos;

public:
	void SetParticleBuffer(StructuredBuffer* _particleBuffer) { m_ParticleBuffer = _particleBuffer; }
	void SetParticleModuleBuffer(StructuredBuffer* _Buffer) { m_ParticleModuleBuffer = _Buffer; }
	void SetParticleSpawnCount(StructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
	void SetParticleWorldPos(Vec3 _worldPos) { m_ParticleWorldPos = _worldPos; }

public:
	virtual HRESULT UpdateData()override;
	virtual void UpdateGroupCount()override;
	virtual void Clear() override;

public:
	ParticleUpdate();
	virtual ~ParticleUpdate();
};

