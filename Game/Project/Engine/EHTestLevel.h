#pragma once

#include "EHLevel.h"

class TestLevel
	:public Level
{
private:

public:
	virtual void Init()override;
	virtual void Tick()override;
	virtual void FinalTick()override;
	virtual void Render()override;

public:
	TestLevel();
	virtual ~TestLevel();
};

