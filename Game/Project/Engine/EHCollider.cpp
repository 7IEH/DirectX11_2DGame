#include "pch.h"
#include "EHCollider.h"

Collider::Collider(COLLIDER_TYPE _type)
	:Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Type(_type)
	, m_CollisionCount(0)
	, m_Enabled(TRUE)
{
}

Collider::~Collider()
{
}

void Collider::OnTriggerEnter(Collider* _other)
{
	m_CollisionCount++;
}

void Collider::OnTriggerStay(Collider* _other)
{
}

void Collider::OnTriggerExit(Collider* _other)
{
	m_CollisionCount--;
}