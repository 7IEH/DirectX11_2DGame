#pragma once

#include "EHComponent.h"

class Script
	:public Component
{
private:
	UINT		m_iScriptType;

public:
	virtual void Tick();

public:
	Script();
	virtual ~Script();
};

