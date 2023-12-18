#include "pch.h"
#include "EHCameraScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

CameraScript::CameraScript()
	:m_Target(nullptr)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::Tick()
{
	if (m_Target != nullptr)
		GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)
		->SetPosition(m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetTransform()->_Position);
}