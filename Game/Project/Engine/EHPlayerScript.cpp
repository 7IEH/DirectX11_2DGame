#include "pch.h"
#include "EHPlayerScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHKeyMgr.h"

#include "EHTimeMgr.h"

PlayerScript::PlayerScript()
	:m_Speed(2.f)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Tick()
{
	Transform* _transform = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = nullptr;
	if (_transform == nullptr)
	{
		return;
	}
	_transformInfo = _transform->GetTransform();
	if (KEY_PRESSED(LEFT))
	{
		_transformInfo->_Position.x -= m_Speed * DT;
	}

	if (KEY_PRESSED(RIGHT))
	{
		_transformInfo->_Position.x += m_Speed * DT;
	}

	if (KEY_PRESSED(UP))
	{
		_transformInfo->_Position.y += m_Speed * DT;
	}

	if (KEY_PRESSED(DOWN))
	{
		_transformInfo->_Position.y -= m_Speed * DT;
	}
}