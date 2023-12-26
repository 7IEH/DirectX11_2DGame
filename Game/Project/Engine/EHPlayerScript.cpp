#include "pch.h"
#include "EHPlayerScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHKeyMgr.h"

#include "EHTimeMgr.h"

PlayerScript::PlayerScript()
	:m_Speed(200.f)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Update()
{
	Transform* _transform = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = nullptr;
	if (_transform == nullptr)
	{
		return;
	}
	Vec4 _pos = _transform->GetRelativePosition();
	if (KEY_PRESSED(LEFT))
	{
		_pos.x -= m_Speed * DT;
	}

	if (KEY_PRESSED(RIGHT))
	{
		_pos.x += m_Speed * DT;
	}

	if (KEY_PRESSED(UP))
	{
		_pos.y += m_Speed * DT;
	}

	if (KEY_PRESSED(DOWN))
	{
		_pos.y -= m_Speed * DT;
	}
	_transform->SetRelativePosition(_pos);
}