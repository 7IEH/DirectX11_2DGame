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

public:
	PlayerScript();
	virtual ~PlayerScript();
};

