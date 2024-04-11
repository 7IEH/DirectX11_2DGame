#include "pch.h"
#include "EHBossUIScript.h"

#include <EHLevelMgr.h>

#include "EHGolemKingScript.h"
#include <EHTimeMgr.h>

BossUIScript::BossUIScript()
	:m_pHealthBar(nullptr)
	, m_pHealthBarDeco(nullptr)
	, m_bDead(FALSE)
	, m_vHPUIScale{}
	, m_vHPUIPos{}
	, m_bAttack(FALSE)
	, m_fAttackTime(0.f)
{
	SetName(L"BossUIScript");
}

BossUIScript::~BossUIScript()
{
}

void BossUIScript::Start()
{
	m_pHealthBar = FIND_OBJECT(L"GUI_Boss_HealthBar");
	m_pHealthBarDeco = FIND_OBJECT(L"GUI_Boss_HealthBarDeco");

	assert(m_pHealthBar);
	assert(m_pHealthBarDeco);

	m_pHealthBar->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));

	m_vHPUIScale = m_pHealthBar->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();
	m_vHPUIPos = m_pHealthBar->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	m_pHealthBar->Enabled(FALSE);
	m_pHealthBarDeco->Enabled(FALSE);
}

void BossUIScript::Update()
{
	if (m_bDead)
	{
		Object::FadeOut(m_pHealthBar, 1.5f);
		Object::FadeOut(m_pHealthBarDeco, 1.5f);

		m_bDead = FALSE;
		return;
	}
	else
	{
		Transform* _pTr = m_pHealthBar->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		int _iCurHP = GetOwner()->GetScript<GolemKingScript>()->GetHp();
		int _iMaxHP = 400;

		float _fRatio = (float)_iCurHP / (float)_iMaxHP;

		float _fDifference = (m_vHPUIScale.x) - (m_vHPUIScale.x * _fRatio);

		Vec4 _vScale = m_vHPUIScale;
		Vec4 _vPos = m_vHPUIPos;
		_vScale.x *= _fRatio;
		_vPos.x -= (_fDifference / 2.f);

		_pTr->SetRelativePosition(_vPos);
		_pTr->SetRelativeScale(_vScale);
	}

	if (m_bAttack)
	{
		m_pHealthBar->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
		m_fAttackTime += DT;
		if (m_fAttackTime >= 1.f)
		{
			m_pHealthBar->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));
			m_bAttack = FALSE;
			m_fAttackTime = 0.f;
		}
	}
}