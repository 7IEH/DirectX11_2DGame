#include "pch.h"
#include "EHGolemKingATK1Script.h"

#include <EHGameObject.h>
#include <EHTimeMgr.h>
#include <EHAssetMgr.h>
#include "EHSpawnEffectScript.h"

GolemKingATK1Script::GolemKingATK1Script()
	:m_vTargetPos{}
	, m_bDown(TRUE)
	, m_bUp(FALSE)
	, m_fAccTime(0.f)
	, m_eState(ATK1State::Idle)
{
	SetName(L"GolemKingATK1Script");
}

GolemKingATK1Script::~GolemKingATK1Script()
{
}


void GolemKingATK1Script::Start()
{
}

void GolemKingATK1Script::Update()
{
	switch (m_eState)
	{
	case ATK1State::Idle:
	{
		Idle();
	}
	break;
	case ATK1State::Attack:
	{
		Attack();
	}
	break;
	case ATK1State::none:
		break;
	default:
		break;
	}
}

void GolemKingATK1Script::Idle()
{
	//GetOwner()->Enabled(FALSE);
}

void GolemKingATK1Script::Attack()
{
	if (m_bDown)
	{
		GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(m_vTargetPos.x, m_vTargetPos.y, -1000.f, 0.f));
		Object::MoveDown(GetOwner(), m_vTargetPos.y, 1000.f);
		m_bDown = FALSE;
	}

	if (!m_bDown && !m_bUp)
	{
		Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
		if (_vPos.y <= m_vTargetPos.y + 1.f)
		{
			m_fAccTime += DT;
			if (m_fAccTime >= 1.f)
			{
				m_fAccTime = 0.f;
				m_bUp = TRUE;
				SpawnCrack();
			}
		}
	}

	if (m_bUp)
	{
		Object::MoveUp(GetOwner(), 1300.f, 1000.f);
		m_eState = ATK1State::Idle;
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_PunchUp_Anim", FALSE);
		m_bDown = TRUE;
		m_bUp = FALSE;
	}
}

void GolemKingATK1Script::SpawnCrack()
{
	Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	GameObject* _pCrack = new GameObject;
	Transform* _pTr = _pCrack->AddComponent<Transform>();
	MeshRenderer* _pRender = _pCrack->AddComponent<MeshRenderer>();

	_vPos.z = 1000.f;
	_pTr->SetRelativePosition(_vPos);
	_pTr->SetRelativeScale(Vec4(95.f, 67.5f, 1.f, 1.f));
	_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Boss_Golem_Attack_Punch_Crack1_Mat"));

	Object::Instantiate(_pCrack, int(LAYER_TYPE::BACKGROUND2));
}