#pragma once
#include "EHScript.h"

class CameraScript
	:public Script
{
private:
	GameObject*		m_Target;
	float			m_CamSpeed;

public:
	virtual void Update() override;
	virtual void LateUpdate() override {};

public:
	void SetTarget(GameObject* _target) { m_Target = _target; }

private:
	void PerpectiveMove();

	void OrthoGraphicMove();

public:
	CameraScript();
	virtual ~CameraScript();
};

