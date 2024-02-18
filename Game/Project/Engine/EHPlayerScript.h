#pragma once
#include "EHScript.h"

class PlayerScript
	:public Script
{
private:
	float m_Speed;

public:
	virtual void Update()override;
	virtual void LateUpdate() override {};
	
	CLONE(PlayerScript)

public:
	PlayerScript();
	virtual ~PlayerScript();
};

