#include "pch.h"
#include "EHGolemTurretScript.h"

#include <EHBehaviour.h>
#include <EHGameObject.h>
#include <EHTimeMgr.h>
#include <EHAnimation2D.h>
#include <EHAssetMgr.h>

#include "EHProjecTileScript.h"

GolemTurretScript::GolemTurretScript()
{
	SetName(L"GolemTurretScript");
	SetHp(100);
	SetDamage(10);
	SetChase(FALSE);
	SetChargeTime(1.5f);
}

GolemTurretScript::~GolemTurretScript()
{
}

void GolemTurretScript::Start()
{
	GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->SetoffSetScale(Vec3(0.05f, 0.05f, 1.f));
}

void GolemTurretScript::Update()
{
	EnemyScript::Update();
}

void GolemTurretScript::Idle()
{
	if (GetChase())
	{
		SetState(EnemyState::Attack);

		Animator2D* _pTurretAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

		switch (GetDir())
		{
		case EnemyDir::UP:
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Attack_Up_Anim");
		}
		break;
		case EnemyDir::DOWN:
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Attack_Down_Anim");
		}
		break;
		case EnemyDir::LEFT:
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Attack_Left_Anim");
		}
		break;
		case EnemyDir::RIGHT:
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Attack_Right_Anim");
		}
		break;
		default:
			break;
		}
	}
}

void GolemTurretScript::Chase()
{
}

void GolemTurretScript::Attack()
{
	Animator2D* _pTurretAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	float _fAccTime = GetAccTime();
	SetAccTime(_fAccTime += DT);
	switch (GetDir())
	{
	case EnemyDir::UP:
	{
		if (L"FSM_Enemy_Golem_Turret_Idle_Up_Anim" != _pTurretAnim->GetCurAnimation2D()->GetName())
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Idle_Up_Anim");
		}
	}
	break;
	case EnemyDir::DOWN:
	{
		if (L"FSM_Enemy_Golem_Turret_Idle_Down_Anim" != _pTurretAnim->GetCurAnimation2D()->GetName())
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Idle_Down_Anim");
		}
	}
	break;
	case EnemyDir::LEFT:
	{
		if (L"FSM_Enemy_Golem_Turret_Idle_Left_Anim" != _pTurretAnim->GetCurAnimation2D()->GetName())
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Idle_Left_Anim");
		}
	}
	break;
	case EnemyDir::RIGHT:
	{
		if (L"FSM_Enemy_Golem_Turret_Idle_Right_Anim" != _pTurretAnim->GetCurAnimation2D()->GetName())
		{
			_pTurretAnim->Play(L"FSM_Enemy_Golem_Turret_Idle_Right_Anim");
		}
	}
	break;
	default:
		break;
	}

	if (_fAccTime >= GetChargeTime())
	{
		Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		LaunchMissile(L"FSM_Enemy_Golem_Turret_Projectile_Anim", L"", Dir(GetDir()), 300.f, 5.f, _pTr->GetRelativePosition(), Vec4(500.f, 500.f, 1.f, 0.f));
		SetAccTime(0.f);
	}

	CaculateDir();
}

void GolemTurretScript::OnTriggerEnter(Collider* _other)
{
	Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_golem_hit.wav", FALSE, 0.5f);
	EnemyScript::OnTriggerEnter(_other);
}