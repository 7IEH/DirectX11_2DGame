#include "pch.h"
#include "EHSlimeHermitScript.h"

#include <EHLevelMgr.h>
#include <EHGameObject.h>
#include <EHTimeMgr.h>
#include <EHAnimation2D.h>
#include "EHProjecTileScript.h"

SlimeHermitScript::SlimeHermitScript()
{
	SetName(L"SlimeHermitScript");
	SetHp(50);
	SetDamage(5);
	SetChase(FALSE);
}

SlimeHermitScript::~SlimeHermitScript()
{
}

void SlimeHermitScript::Start()
{
	EnemyScript::Start();
	GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->SetoffSetScale(Vec3(0.2f, 0.2f, 1.f));
	m_pCollider = new GameObject;
	Transform* _pColTr = m_pCollider->AddComponent<Transform>();
	Collider* _pColCol = m_pCollider->AddComponent<Collider2D>();
	_pColTr->SetRelativeScale(Vec4(60.f, 60.f, 1.f, 1.f));
	_pColCol->Enabled(FALSE);


	ProjecTileScript* _pScript = m_pCollider->AddComponent<ProjecTileScript>();
	_pScript->SetDamage(10.f);
	_pScript->SetNotDir(TRUE);

	LevelMgr::GetInst()->GetCurLevel()->AddObject(m_pCollider, LAYER_TYPE::ENEMY_PROJECTILE);

	GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->SetCurAnimation2D(L"FSM_Enemy_SlimeHermit_Walk_Up_Anim");
}

void SlimeHermitScript::Update()
{
	EnemyScript::Update();
}

void SlimeHermitScript::Idle()
{
	if (GetChase())
	{
		SetState(EnemyState::Chase);
	}
}

void SlimeHermitScript::Chase()
{
	m_fAccTime = 0.f;

	Transform* _pPlayerTr = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")
		->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Animator2D* _pAnimator = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pPlayerTr);

	Transform* _pSlimeTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _vPlayerPos = _pPlayerTr->GetRelativePosition();
	Vec4 _vSlimePos = _pSlimeTr->GetRelativePosition();

	Vec4 _vDifference = _vPlayerPos - _vSlimePos;

	float _fDistance = sqrt(_vDifference.x * _vDifference.x + _vDifference.y * _vDifference.y);

	_vSlimePos.x += _vDifference.x / _fDistance * DT * 50.f;
	_vSlimePos.y += _vDifference.y / _fDistance * DT * 50.f;

	_pSlimeTr->SetRelativePosition(_vSlimePos);

	switch (GetDir())
	{
	case EnemyDir::UP:
	{
		if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Walk_Up_Anim")
		{
			_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Walk_Up_Anim", TRUE);
		}
	}
	break;
	case EnemyDir::DOWN:
	{
		if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Walk_Down_Anim")
		{
			_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Walk_Down_Anim", TRUE);
		}
	}
	break;
	case EnemyDir::LEFT:
	{
		if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Walk_Left_Anim")
		{
			_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Walk_Left_Anim", TRUE);
		}
	}
	break;
	case EnemyDir::RIGHT:
	{
		if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Walk_Right_Anim")
		{
			_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Walk_Right_Anim", TRUE);
		}
	}
	break;
	default:
		break;
	}

	if (_fDistance <= 100.f)
	{
		SetState(EnemyState::Attack);
		m_eAttackDir = GetDir();
	}

	CaculateDir();
}

void SlimeHermitScript::Attack()
{
	Animator2D* _pAnimator = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Transform* _pColTr = m_pCollider->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	m_fAccTime += DT;
	if (m_fAccTime >= 1.5f)
	{
		m_pCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
		switch (m_eAttackDir)
		{
		case EnemyDir::UP:
		{
			_vPos.y += 100.f;
			if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Attack_Up_Anim")
			{
				_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Attack_Up_Anim", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->GetName() == L"FSM_Enemy_SlimeHermit_Attack_Up_Anim"
				&& _pAnimator->GetCurAnimation2D()->IsFinish())
			{
				SetState(EnemyState::Chase);
				m_pCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}
		}
		break;
		case EnemyDir::DOWN:
		{
			_vPos.y -= 100.f;
			if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Attack_Down_Anim")
			{
				_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Attack_Down_Anim", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->GetName() == L"FSM_Enemy_SlimeHermit_Attack_Down_Anim"
				&& _pAnimator->GetCurAnimation2D()->IsFinish())
			{
				SetState(EnemyState::Chase);
				m_pCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}
		}
		break;
		case EnemyDir::LEFT:
		{
			_vPos.x -= 100.f;
			if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Attack_Left_Anim")
			{
				_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Attack_Left_Anim", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->GetName() == L"FSM_Enemy_SlimeHermit_Attack_Left_Anim"
				&& _pAnimator->GetCurAnimation2D()->IsFinish())
			{
				SetState(EnemyState::Chase);
				m_pCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}
		}
		break;
		case EnemyDir::RIGHT:
		{
			_vPos.x += 100.f;
			if (_pAnimator->GetCurAnimation2D()->GetName() != L"FSM_Enemy_SlimeHermit_Attack_Right_Anim")
			{
				_pAnimator->Play(L"FSM_Enemy_SlimeHermit_Attack_Right_Anim", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->GetName() == L"FSM_Enemy_SlimeHermit_Attack_Right_Anim"
				&& _pAnimator->GetCurAnimation2D()->IsFinish())
			{
				SetState(EnemyState::Chase);
				m_pCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}
		}
		break;
		default:
			break;
		}

		_pColTr->SetRelativePosition(_vPos);
	}
}

void SlimeHermitScript::OnTriggerEnter(Collider* _other)
{
	Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_golem_hit.wav", TRUE, 0.5f);
	EnemyScript::OnTriggerEnter(_other);
}