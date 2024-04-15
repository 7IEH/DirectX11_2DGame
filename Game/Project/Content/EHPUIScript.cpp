#include "pch.h"
#include "EHPUIScript.h"

#include <EHLevelMgr.h>
#include <EHAssetMgr.h>
#include "EHItemMgr.h"

#include <EHTimeMgr.h>

PUIScript::PUIScript()
	:m_pHPUI(nullptr)
	, m_pHPHeartUI(nullptr)
	, m_pWeaponCircle1(nullptr)
	, m_pWeaponCircle2(nullptr)
	, m_pSetIcon1(nullptr)
	, m_pSetIcon2(nullptr)
	, m_pPlayerPref(nullptr)
	, m_pWeaponIcon1(nullptr)
	, m_pWeaponIcon2(nullptr)
	, m_pUseItemIcon(nullptr)
	, m_pHPMaxText(nullptr)
	, m_pHPCurText(nullptr)
	, m_bDamaged(FALSE)
	, m_fAccTime(0.f)
	, m_iPrevHP(100)
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

	m_pWeaponIcon1 = FIND_OBJECT(L"UI_Player_HUD_Weapon1");
	m_pWeaponIcon2 = FIND_OBJECT(L"UI_Player_HUD_Weapon2");
	m_pUseItemIcon = FIND_OBJECT(L"UI_Player_HUD_UseItem");

	m_pHPMaxText = FIND_OBJECT(L"UI_Player_HUD_MaxText");
	m_pHPCurText = FIND_OBJECT(L"UI_Player_HUD_CurText");

	m_pPlayerPref = RecordManager::GetInst()->GetCurrentPlayerPref();

	assert(m_pHPUI);
	assert(m_pHPHeartUI);
	assert(m_pWeaponCircle1);
	assert(m_pWeaponCircle2);
	assert(m_pSetIcon1);
	assert(m_pSetIcon2);
	assert(m_pPlayerPref);
	assert(m_pWeaponIcon1);
	assert(m_pWeaponIcon2);
	assert(m_pUseItemIcon);
	assert(m_pHPMaxText);
	assert(m_pHPCurText);

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

	/***************
	| HP UI Icon
	***************/
	Transform* _pTr = m_pHPUI->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	int _iCurHP = m_pPlayerPref->_iCurHp;
	int _iMaxHP = m_pPlayerPref->_iMaxHp;

	if (m_iPrevHP != _iCurHP)
	{
		m_bDamaged = TRUE;
	}

	m_iPrevHP = m_pPlayerPref->_iCurHp;

	float _fRatio = (float)_iCurHP / (float)_iMaxHP;

	float _fDifference = (m_vHPUIScale.x) - (m_vHPUIScale.x * _fRatio);

	Vec4 _vScale = m_vHPUIScale;
	Vec4 _vPos = m_vHPUIPos;
	_vScale.x *= _fRatio;
	_vPos.x -= (_fDifference / 2.f);

	_pTr->SetRelativePosition(_vPos);
	_pTr->SetRelativeScale(_vScale);

	// Text
	m_pHPMaxText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(std::to_wstring(m_pPlayerPref->_iMaxHp));
	m_pHPCurText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(std::to_wstring(m_pPlayerPref->_iCurHp));

	if (m_bDamaged)
	{
		m_fAccTime += DT;

		m_pHPUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
		m_pHPHeartUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));

		if (m_fAccTime >= 0.5f)
		{
			m_bDamaged = FALSE;
			m_fAccTime = 0.f;
		}
	}
	else
	{
		m_pHPUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));
		m_pHPHeartUI->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 0.f, 0.f, 1.f));
	}

	/**************
	| Weapon Icon
	**************/
	if (m_pPlayerPref->_iCurWeapon == 0)
	{
		m_pWeaponIcon1->Enabled(TRUE);
		m_pWeaponIcon1->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(ItemMgr::GetInst()->GetMaterial(int(m_pPlayerPref->_iWeapon1))));

		m_pWeaponIcon2->Enabled(FALSE);

		m_pWeaponCircle1->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
		m_pSetIcon1->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
		m_pWeaponCircle2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));
		m_pSetIcon2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));
	}
	else
	{
		m_pWeaponIcon1->Enabled(FALSE);

		m_pWeaponIcon2->Enabled(TRUE);
		m_pWeaponIcon2->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(ItemMgr::GetInst()->GetMaterial(int(m_pPlayerPref->_iWeapon2))));


		m_pWeaponCircle1->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));
		m_pSetIcon1->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));
		m_pWeaponCircle2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
		m_pSetIcon2->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 1.f));
	}

	/**************
	| Util Icon
	**************/
	if (m_pPlayerPref->_iUseItem == ITEM::NO_ITEM)
	{
		m_pUseItemIcon->Enabled(FALSE);
	}
	else
	{
		m_pUseItemIcon->Enabled(TRUE);
		m_pUseItemIcon->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(ItemMgr::GetInst()->GetMaterial(int(m_pPlayerPref->_iUseItem))));
	}
}