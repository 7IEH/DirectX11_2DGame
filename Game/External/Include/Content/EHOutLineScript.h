#pragma once

#include "EHScript.h"

class OutLineScript
	:public Script
{
private:
	GameObject* m_Owner;

public:
	void SetFollowOwner(GameObject* _owner) { m_Owner = _owner; }

public:
	virtual void Update() override;

public:
	OutLineScript();
	virtual ~OutLineScript();
};

