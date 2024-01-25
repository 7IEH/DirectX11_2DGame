#pragma once
#include "EHRenderer.h"

#include "EHStructuredBuffer.h"

class ParticleSystem
	:public Renderer
{
private:
	StructuredBuffer* m_ParticleBuffer;
	UINT			  m_MaxParticleCount;

public:
	virtual void UpdateData() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	ParticleSystem();
	virtual ~ParticleSystem();
};

