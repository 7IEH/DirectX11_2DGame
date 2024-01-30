#pragma once
#include "EHRenderer.h"

#include "EHStructuredBuffer.h"
#include "EHParticleUpdate.h"
class ParticleSystem
	:public Renderer
{
private:
	StructuredBuffer*	m_ParticleBuffer;
	UINT				m_MaxParticleCount;

	tParticleModule		m_Module;
	StructuredBuffer*	m_ParticleModuleBuffer;

	StructuredBuffer*	m_SpawnCountBuffer;

	Ptr<ParticleUpdate>	m_CSParticleUpdate;

	float				m_Time;
public:
	virtual void UpdateData() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	ParticleSystem();
	virtual ~ParticleSystem();
};

