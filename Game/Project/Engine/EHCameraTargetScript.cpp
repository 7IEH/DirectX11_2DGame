#include "pch.h"
#include "EHCameraTargetScript.h"

#include "EHGameObject.h"

#include "EHTransform.h"

CameraTargetScript::CameraTargetScript()
	:m_Target(nullptr)
{
}

CameraTargetScript::~CameraTargetScript()
{
}


void CameraTargetScript::Tick()
{
	GameObject* _owner = GetOwner();
	Transform* _tr = _owner->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_tr == nullptr)
		return;

	Transform* _targettr = m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_targettr == nullptr)
		return;

	Vec4 _temp = Vec4(_targettr->GetPosition().x, _targettr->GetPosition().y, -10.f, 0.f);
	_tr->SetPosition(_temp);
}