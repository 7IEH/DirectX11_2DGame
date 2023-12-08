#include "pch.h"
#include "EHPlayer.h"

#include "EHTransform.h"

#include "EHKeyMgr.h"
#include "EHTimeMgr.h"

void Player::Tick()
{
	GameObject::Tick();
	
	Transform* _transform = GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = _transform->GetTransform();

	if (KEY_PRESSED(A))
	{
		_transformInfo->_Position.x -= DT;
	}

	if (KEY_PRESSED(D))
	{
		_transformInfo->_Position.x += DT;
	}

	if (KEY_PRESSED(W))
	{
		_transformInfo->_Position.y += DT;
	}

	if (KEY_PRESSED(S))
	{
		_transformInfo->_Position.y -= DT;
	}
}

void Player::Render()
{
	GameObject::Render();
}
