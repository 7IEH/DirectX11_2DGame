#include "pch.h"
#include "EHCameraTargetScript.h"

#include "EHGameObject.h"

#include "EHTransform.h"
#include <EHLevelMgr.h>
#include <EHTimeMgr.h>

CameraTargetScript::CameraTargetScript()
	: m_Target(nullptr)
	, m_fCamSpeed(2.f)
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

	if (m_Target == nullptr)
		return;

	Transform* _targettr = m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	
	if (_targettr == nullptr)
		return;

	Vec2 _CurPos = Vec2(_tr->GetRelativePosition().x, _tr->GetRelativePosition().y);
	Vec2 _targetPos = Vec2(_targettr->GetRelativePosition().x, _targettr->GetRelativePosition().y);

	_CurPos = _CurPos.Lerp(_CurPos, _targetPos, DT* m_fCamSpeed);

	Vec4 _temp = Vec4(_CurPos.x, _CurPos.y, -2000.f, 0.f);

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

	if (L"GolemDungeonBossScene" == LevelMgr::GetInst()->GetCurLevel()->GetName())
	{
		if (_temp.x >= 640.f)
		{
			_temp.x = 640.f;
		}

		if (_temp.y >= 460.f)
		{
			_temp.y = 460.f;
		}

		if (_temp.x <= -640.f)
		{
			_temp.x = -640.f;
		}

		if (_temp.y <= -460.f)
		{
			_temp.y = -460.f;
		}
	}

	_tr->SetRelativePosition(_temp);
}