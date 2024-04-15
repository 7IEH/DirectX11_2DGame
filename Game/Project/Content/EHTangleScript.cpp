#include "pch.h"
#include "EHTangleScript.h"

#include <EHLevelMgr.h>
#include <EHGameObject.h>
#include <EHTimeMgr.h>

TangleScript::TangleScript()
{
	SetName(L"TangleScript");
	SetHp(20);
	SetDamage(5);
	SetChase(FALSE);
}

TangleScript::~TangleScript()
{
}

void TangleScript::Update()
{
	EnemyScript::Update();
}

void TangleScript::Start()
{
	GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->SetoffSetScale(Vec3(0.07f, 0.07f, 1.f));
}

void TangleScript::Idle()
{
	if (GetChase())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"FSM_Enemy_Tangle_Cycle_Anim");
		SetState(EnemyState::Chase);
	}
}

void TangleScript::Chase()
{
	Transform* _pPlayerTr = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")
		->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	assert(_pPlayerTr);

	Transform* _pTangleTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _vPlayerPos = _pPlayerTr->GetRelativePosition();
	Vec4 _vTanglePos = _pTangleTr->GetRelativePosition();

	Vec4 _vDifference = _vPlayerPos - _vTanglePos;

	float _fDistance = sqrt(_vDifference.x* _vDifference.x + _vDifference.y * _vDifference.y);

	_vTanglePos.x += _vDifference.x / _fDistance * DT * 50.f;
	_vTanglePos.y += _vDifference.y / _fDistance * DT * 50.f;

	_pTangleTr->SetRelativePosition(_vTanglePos);
}

void TangleScript::OnTriggerEnter(Collider* _other)
{
	Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_tangle_hit.wav", TRUE, 0.5f);
	EnemyScript::OnTriggerEnter(_other);
}