#pragma once

#include "EHScript.h"

class Light2DScript
	:public Script
{
private:

public:
	virtual void Update() override;

	CLONE(Light2DScript)

public:
	Light2DScript();
	virtual ~Light2DScript();
};

