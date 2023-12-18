#pragma once
#include "EHScript.h"

class PlayerScript
	:public Script
{
private:
	float m_Speed;

public:
	virtual void Tick()override;
	virtual void FinalTick() override {};

public:
	PlayerScript();
	virtual ~PlayerScript();
};

