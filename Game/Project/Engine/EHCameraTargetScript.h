#pragma once
#include "EHScript.h"

class GameObject;
class CameraTargetScript
	:public Script
{
private:
	GameObject* m_Target;

public:
	GameObject* GetTarget() { return m_Target; }
	void SetTarget(GameObject* _target) { m_Target = _target; }

public:
	virtual void Tick()override;

public:
	CameraTargetScript();
	virtual ~CameraTargetScript();
};

