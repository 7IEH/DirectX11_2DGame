#include "pch.h"
#include "EHLIght2D.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHRenderMgr.h"

#include "EHGameObject.h"

LIght2D::LIght2D()
	:Component(COMPONENT_TYPE::LIGHT2D)
{
}

LIght2D::~LIght2D()
{
}

void LIght2D::LateUpdate()
{
	GameObject* _light = GetOwner();
	Transform* _lightTr = _light->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	m_LightInfo._Position = Vec3(_lightTr->GetRelativePosition());
	m_LightInfo._LightDir = _lightTr->GetWorldDir(DIRECTION_TYPE::RIGHT);

	RenderMgr::GetInst()->RegisterLight2D(GetOwner());
}
