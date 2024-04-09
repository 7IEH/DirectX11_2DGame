#include "pch.h"
#include "EHCameraTargetScript.h"

#include "EHGameObject.h"

#include "EHTransform.h"
#include <EHLevelMgr.h>

CameraTargetScript::CameraTargetScript()
	:m_Target(nullptr)
{
	SetName(L"CameraTargetScript");
}

CameraTargetScript::~CameraTargetScript()
{
}


void CameraTargetScript::Update()
{
	GameObject* _owner = GetOwner();
	Transform* _tr = _owner->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_tr == nullptr)
		return;

	m_Target = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");

	Transform* _targettr = m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_targettr == nullptr)
		return;

	Vec4 _temp = Vec4(_targettr->GetRelativePosition().x, _targettr->GetRelativePosition().y, -2000.f, 0.f);
	

	if (L"TownScene" == LevelMgr::GetInst()->GetCurLevel()->GetName())
	{
		if (_temp.x >= 826.25f)
		{
			_temp.x = 826.25f;
		}

		if (_temp.y >= 888.75f)
		{
			_temp.y = 888.75f;
		}

		if (_temp.x <= -826.25f)
		{
			_temp.x = -826.25f;
		}

		if (_temp.y <= -888.75f)
		{
			_temp.y = -888.75f;
		}
	}

	if (L"DungeonEntranceScene" == LevelMgr::GetInst()->GetCurLevel()->GetName())
	{
		if (_temp.x >= 705.f)
		{
			_temp.x = 705.f;
		}

		if (_temp.y >= 835.f)
		{
			_temp.y = 835.f;
		}

		if (_temp.x <= -705.f)
		{
			_temp.x = -705.f;
		}

		if (_temp.y <= -835.f)
		{
			_temp.y = -835.f;
		}
	}

	_tr->SetRelativePosition(_temp);
}