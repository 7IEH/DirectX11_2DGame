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
	if (KEY_PRESSED(A))
	{
		_transformInfo->_Position.x -= m_Speed * DT;
	}

	if (KEY_PRESSED(D))
	{
		_transformInfo->_Position.x += m_Speed * DT;
	}

	if (KEY_PRESSED(W))
	{
		_transformInfo->_Position.y += m_Speed * DT;
	}

	if (KEY_PRESSED(S))
	{
		_transformInfo->_Position.y -= m_Speed * DT;
	}
}