#pragma once

#include "EHScript.h"

class TestScript
	:public Script
{
private:
	virtual void Update() override;
	float m_AccTime;
	
public:
	CLONE(TestScript)

public:
	TestScript();
	virtual ~TestScript();
};

