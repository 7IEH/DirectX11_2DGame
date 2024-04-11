#include "pch.h"
#include "EHGolemKingATK4Script.h"

#include <EHGameObject.h>
#include <EHRigidBody.h>

GolemKingATK4Script::GolemKingATK4Script()
{
	SetName(L"GolemKingATK4Script");
}

GolemKingATK4Script::~GolemKingATK4Script()
{
}

void GolemKingATK4Script::OnTriggerEnter(Collider* _other)
{
	//_other->GetOwner()->GetScript<PlayerScript>()->SetKnockBack(TRUE);

	Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Vec4 _vPlayerPos = _other->GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	Vec2 _vDir = Vec2(_vPlayerPos.x - _vPos.x, _vPlayerPos.y - _vPos.y);
	_vDir.Normalize();
	_other->GetOwner()->GetComponent<RigidBody>(COMPONENT_TYPE::RIGIDBODY)->SetVeclocity(Vec2(_vDir.x * 500.f, _vDir.y * 500.f));
}