#pragma once
#include "EHScript.h"

class GameObject;
class CameraTargetScript
	:public Script
{
private:
	GameObject* m_Target;

	float		m_fCamSpeed;
public:
	GameObject* GetTarget() { return m_Target; }
	void SetTarget(GameObject* _target) { m_Target = _target; }

	void SetCamSpeed(float _fCamSpeed) { m_fCamSpeed = _fCamSpeed; }

public:
	virtual void Update()override;

	CLONE(CameraTargetScript);

public:
	CameraTargetScript();
	virtual ~CameraTargetScript();
};

