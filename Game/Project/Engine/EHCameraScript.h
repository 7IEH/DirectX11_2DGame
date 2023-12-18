#pragma once
#include "EHScript.h"

class CameraScript
	:public Script
{
private:
	GameObject* m_Target;

public:
	virtual void Tick() override;
	virtual void FinalTick() override {};

public:
	void SetTarget(GameObject* _target) { m_Target = _target; }

public:
	CameraScript();
	virtual ~CameraScript();
};

