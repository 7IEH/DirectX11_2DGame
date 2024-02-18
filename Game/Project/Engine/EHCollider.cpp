#include "pch.h"
#include "EHCollider.h"

#include "EHGameObject.h"
#include "EHScript.h"

#include "EHDevice.h"

Collider::Collider(COLLIDER_TYPE _type)
	:Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Type(_type)
	, m_CollisionCount(0)
	, m_Enabled(TRUE)
	, m_Absolute(FALSE)
{
}

Collider::Collider(const Collider& _origin)
	:Component(_origin)
	, m_CollisionCount(0)
	, m_Type(_origin.m_Type)
	, m_Enabled(_origin.m_Enabled)
	, m_Absolute(_origin.m_Absolute)
{
}

Collider::~Collider()
{
}

void Collider::OnTriggerEnter(Collider* _other)
{
	vector<Script*>& _script = GetOwner()->GetScript();
	if (_script.empty())
	{
		return;
	}

	for (int i = 0;i < _script.size();i++)
	{
		_script[i]->OnTriggerEnter(_other);
	}
}

void Collider::OnTriggerStay(Collider* _other)
{
	vector<Script*>& _script = GetOwner()->GetScript();
	if (_script.empty())
	{
		return;
	}

	for (int i = 0;i < _script.size();i++)
	{
		_script[i]->OnTriggerStay(_other);
	}
}

void Collider::OnTriggerExit(Collider* _other)
{
	vector<Script*>& _script = GetOwner()->GetScript();
	if (_script.empty())
	{
		return;
	}

	for (int i = 0;i < _script.size();i++)
	{
		_script[i]->OnTriggerExit(_other);
	}
}