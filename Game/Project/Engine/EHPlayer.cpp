#include "pch.h"
#include "EHPlayer.h"

#include "EHTransform.h"

#include "EHKeyMgr.h"
#include "EHTimeMgr.h"

void Player::Init()
{
}

void Player::Tick()
{
	GameObject::Tick();

	float _Speed = 3.f;

	Transform* _transform = GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = nullptr;
	if (_transform == nullptr)
	{
		return;
	}
	_transformInfo = _transform->GetTransform();
	if (KEY_PRESSED(A))
	{
		_transformInfo->_Position.x -= _Speed * DT;
	}

	if (KEY_PRESSED(D))
	{
		_transformInfo->_Position.x += _Speed * DT;
	}

	if (KEY_PRESSED(W))
	{
		_transformInfo->_Position.y += _Speed * DT;
	}

	if (KEY_PRESSED(S))
	{
		_transformInfo->_Position.y -= _Speed * DT;
	}
}

void Player::FinalTick()
{
	GameObject::FinalTick();
}

void Player::Render()
{
	GameObject::Render();
}
