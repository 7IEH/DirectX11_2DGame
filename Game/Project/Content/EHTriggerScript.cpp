#include "pch.h"
#include "EHTriggerScript.h"

#include "EHGameObject.h"
#include "EHDebugMgr.h"

#include "EHRenderMgr.h"

#include "EHLevelMgr.h"

TriggerScript::TriggerScript()
	:m_TriggerType(TRIGGER_TYPE::END)
{
	SetName(L"TriggerScript");
}

TriggerScript::~TriggerScript()
{
}

void TriggerScript::Awake()
{
}

void TriggerScript::OnTriggerEnter(Collider* _other)
{
	GameObject* _player = _other->GetOwner();
	if (_player->GetLayerType() != LAYER_TYPE::PLAYER)
		return;
	Transform* _playertr = _player->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	switch (m_TriggerType)
	{
	case TRIGGER_TYPE::MOVE_TRIGGER:
	{
		// 해당 ROOM에 몬스터가 남아 있을시 COLLIDER ENABLED

		// _other(player) 위치 변경
		if (_other->GetOwner()->GetLayerType() == LAYER_TYPE::PLAYER)
		{
			vector<GameObject*> _object = LevelMgr::GetInst()->GetCurLevel()->GetLayer(LAYER_TYPE::CAMERA)->GetLayerObject();
			m_Cam = _object[0];

			Transform* _playertr = _other->GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			_playertr->SetRelativePosition(m_PlayerPos);
			Vec4 _campos = m_Cam->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			_campos.x = m_PlayerPos.x;
			_campos.y = m_PlayerPos.y;

			m_Cam->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_campos);

			DebugMgr::GetInst()->Log("Test");
		}
		// _other(camera move)
	}
		break;
	case TRIGGER_TYPE::END:
		break;
	default:
		break;
	}
}