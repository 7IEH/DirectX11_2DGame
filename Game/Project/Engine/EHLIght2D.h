#pragma once
#include "EHComponent.h"

class LIght2D
	:public Component
{
private:
	tLight					m_Light;

	DirectinalLight			m_DL;
	PointLight				m_PL;
	SpotLight				m_SL;

public:
	virtual void LateUpdate() override;
	
	void SetDirectionLight(DirectinalLight _dl) { m_DL = _dl; }
	void SetPointLight(PointLight _pl) { m_PL = _pl; }
	void SetSpotLight(SpotLight _sl) { m_SL = _sl; }

public:
	LIght2D();
	virtual ~LIght2D();
};

