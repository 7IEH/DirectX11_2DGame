#include "pch.h"
#include "EHTriggerScript.h"

#include "EHGameObject.h"

TriggerScript::TriggerScript()
	:m_Type(TRIGGER_TYPE::END)
	,m_Room(ROOM_TYPE::END)
	,m_Dir(SPAWN_TYPE::END)
{
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

	switch (m_Type)
	{
	case TRIGGER_TYPE::MOVE_TRIGGER:
	{
		// 해당 ROOM에 몬스터가 남아 있을시 COLLIDER ENABLED

		// _other(player) 위치 변경
		if (m_Dir == SPAWN_TYPE::RIGHT)
		{
				
		}
		else if (m_Dir == SPAWN_TYPE::LEFT)
		{

		}
		else if(m_Dir == SPAWN_TYPE::TOP)
		{

		}
		else
		{

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