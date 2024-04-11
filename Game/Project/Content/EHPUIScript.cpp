#include "pch.h"
#include "EHPUIScript.h"

#include <EHLevelMgr.h>

PUIScript::PUIScript()
	:m_pHPUI(nullptr)
	, m_pHPHeartUI(nullptr)
	, m_pWeaponCircle1(nullptr)
	, m_pWeaponCircle2(nullptr)
	, m_pSetIcon1(nullptr)
	, m_pSetIcon2(nullptr)
	, m_pPlayerPref(nullptr)
{
	SetName(L"PUIScript");
}

PUIScript::~PUIScript()
{
}

void PUIScript::Awake()
{
	m_pHPUI = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_HealthBar");
	m_pHPHeartUI = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_HealthBar_Heart");
	m_pWeaponCircle1 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_BaseCircle2");
	m_pWeaponCircle2 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_BaseCircle3");
	m_pSetIcon1 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_SetIcon1");
	m_pSetIcon2 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_HUD_SetIcon2");

	m_pPlayerPref = RecordManager::GetInst()->GetCurrentPlayerPref();

	assert(m_pHPUI);
	assert(m_pHPHeartUI);
	assert(m_pWeaponCircle1);
	assert(m_pWeaponCircle2);
	assert(m_pSetIcon1);
	assert(m_pSetIcon2);
	assert(m_pPlayerPref);

	m_pHPUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));
	m_pHPHeartUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));
	m_pWeaponCircle2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));
	m_pSetIcon2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));

	m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(-522.284f, 388.f, -2.f, 0.f));
	m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativeScale(Vec4(168.f, 29.f, 1.f, 1.f));

	m_vHPUIScale = m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();
	m_vHPUIPos = m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
}

void PUIScript::Update()
{
	if (nullptr == m_pHPUI)
		return;

	Transform* _pTr = m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	int _iCurHP = m_pPlayerPref->_iCurHp;
	int _iMaxHP = m_pPlayerPref->_iMaxHp;

	float _fRatio = (float)_iCurHP / (float)_iMaxHP;

	float _fDifference = (m_vHPUIScale.x) - (m_vHPUIScale.x * _fRatio);

	Vec4 _vScale = m_vHPUIScale;
	Vec4 _vPos = m_vHPUIPos;
	_vScale.x *= _fRatio;
	_vPos.x -= (_fDifference / 2.f);

	_pTr->SetRelativePosition(_vPos);
	_pTr->SetRelativeScale(_vScale);
}