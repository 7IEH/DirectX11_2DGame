#include "pch.h"
#include "EHGolemButlerScript.h"

#include <EHGameObject.h>
#include <EHAnimation2D.h>
#include <EHLevelMgr.h>

GolemButlerScript::GolemButlerScript()
{
	SetName(L"GolemButlerScript");
	SetHp(100);
	SetDamage(10);
	SetChase(FALSE);
}

GolemButlerScript::~GolemButlerScript()
{
}

void GolemButlerScript::Update()
{
	EnemyScript::Update();
}

void GolemButlerScript::Idle()
{
	if (GetChase())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"FSM_Enemy_Golem_Butler_Idle_Up_Anim");
		SetState(EnemyState::Chase);
	}
}

void GolemButlerScript::Chase()
{
	if (GetChase())
	{
		Animator2D* _pGolemButlerAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

		switch (GetDir())
		{
		case EnemyDir::UP:
		{
			if (L"FSM_Enemy_Golem_Butler_Idle_Up_Anim" != _pGolemButlerAnim->GetCurAnimation2D()->GetName())
			{
				_pGolemButlerAnim->Play(L"FSM_Enemy_Golem_Butler_Idle_Up_Anim");
			}
		}
		break;
		case EnemyDir::DOWN:
		{
			if (L"FSM_Enemy_Golem_Butler_Idle_Down_Anim" != _pGolemButlerAnim->GetCurAnimation2D()->GetName())
			{
				_pGolemButlerAnim->Play(L"FSM_Enemy_Golem_Butler_Idle_Down_Anim");
			}
		}
		break;
		case EnemyDir::RIGHT:
		{
			if (L"FSM_Enemy_Golem_Butler_Idle_Right_Anim" != _pGolemButlerAnim->GetCurAnimation2D()->GetName())
			{
				_pGolemButlerAnim->Play(L"FSM_Enemy_Golem_Butler_Idle_Right_Anim");
			}
		}
		break;
		case EnemyDir::LEFT:
		{
			if (L"FSM_Enemy_Golem_Butler_Idle_Left_Anim" != _pGolemButlerAnim->GetCurAnimation2D()->GetName())
			{
				_pGolemButlerAnim->Play(L"FSM_Enemy_Golem_Butler_Idle_Left_Anim");
			}
		}
		break;

		default:
			break;
		}
	}

	CaculateDir();
}

void GolemButlerScript::Attack()
{
}

void GolemButlerScript::OnTriggerEnter(Collider* _other)
{
	EnemyScript::OnTriggerEnter(_other);
}

void GolemButlerScript::OnTriggerExit(Collider* _other)
{
}