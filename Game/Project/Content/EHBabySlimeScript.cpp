#include "pch.h"
#include "EHBabySlimeScript.h"

#include <EHLevelMgr.h>
#include <EHGameObject.h>
#include <EHTimeMgr.h>

BabySlimeScript::BabySlimeScript()
{
	SetName(L"BabySlimeScript");
	SetHp(30);
	SetDamage(5);
	SetChase(FALSE);
}

BabySlimeScript::~BabySlimeScript()
{
}

void BabySlimeScript::Start()
{
	EnemyScript::Start();
	GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->SetoffSetScale(Vec3(0.05f, 0.05f, 1.f));
}

void BabySlimeScript::Update()
{
	EnemyScript::Update();
}

void BabySlimeScript::Idle()
{
	if (GetChase())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"FSM_Enemy_Slime_Baby_Walk_Anim");
		SetState(EnemyState::Chase);
	}
}

void BabySlimeScript::Chase()
{
	Transform* _pPlayerTr = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")
		->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	assert(_pPlayerTr);

	Transform* _pSlimeTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _vPlayerPos = _pPlayerTr->GetRelativePosition();
	Vec4 _vSlimePos = _pSlimeTr->GetRelativePosition();

	Vec4 _vDifference = _vPlayerPos - _vSlimePos;

	float _fDistance = sqrt(_vDifference.x * _vDifference.x + _vDifference.y * _vDifference.y);

	_vSlimePos.x += _vDifference.x / _fDistance * DT * 50.f;
	_vSlimePos.y += _vDifference.y / _fDistance * DT * 50.f;

	_pSlimeTr->SetRelativePosition(_vSlimePos);
}

void BabySlimeScript::OnTriggerEnter(Collider* _other)
{
	Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_babyslime_hit.wav", TRUE, 0.5f);
	EnemyScript::OnTriggerEnter(_other);
}