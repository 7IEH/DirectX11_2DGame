#include "pch.h"
#include "EHLight2DScript.h"

#include "EHKeyMgr.h"
#include "EHTimeMgr.h"

#include "EHGameObject.h"

Light2DScript::Light2DScript()
{
}

Light2DScript::~Light2DScript()
{
}

void Light2DScript::Update()
{
	Transform* tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec3 _degree = tr->GetRelativeRotation();

	LIght2D* light = GetOwner()->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);
	float _angle = light->GetAngle();
	if (KEY_PRESSED(KEY::R))
	{
		_degree.z += DT * 100.f;
	}

	if (KEY_PRESSED(KEY::I))
	{
		_angle += DT * 30.f;
	}
	light->SetAngle(_angle);

	tr->SetRelativeRotation(_degree);
}