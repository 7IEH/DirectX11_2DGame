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

	Ptr<Sprite>			m_ParticleSprite;
	Ptr<Sprite>			m_NoiseSprite;

	float				m_Time;

public:
	tParticleModule	GetModule() { return m_Module; }
	void SetModule(tParticleModule _module) { m_Module = _module; }

	void SetParticleSprite(Ptr<Sprite> _particleSprtie) { m_ParticleSprite = _particleSprtie; }
	Ptr<Sprite> GetParticleSprite() { return m_ParticleSprite; }

	void SetNoiseSprite(Ptr<Sprite> _noiseSprite) { m_NoiseSprite = _noiseSprite; }
	Ptr<Sprite> GetNoiseSprite() { return m_NoiseSprite; }


	void SetTime(float _time) { m_Time = _time; }
	float GetTime() { return m_Time; }

public:
	virtual void UpdateData() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	CLONE(ParticleSystem)

public:
	ParticleSystem();
	ParticleSystem(const ParticleSystem& _origin);
	virtual ~ParticleSystem();
};

