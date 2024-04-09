#pragma once

#include "EHItemScript.h"

class PortionScript
	:public ItemScript
{
private:

public:
	virtual void Use()override;

public:
	CLONE(PortionScript);

public:
	PortionScript();
	virtual ~PortionScript();
};

