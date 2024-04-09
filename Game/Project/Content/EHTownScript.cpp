#include "pch.h"
#include "EHTownScript.h"

#include <EHLevelMgr.h>
#include <EHCollisionMgr.h>
#include "EHTriggerScript.h"
#include <EHTimeMgr.h>

TownScript::TownScript()
	:m_pObject_Village_House5(nullptr)
	, m_pObject_Village_Rival_Fountaion(nullptr)
	, m_pObject_Village_Well(nullptr)
	, m_pObject_Village_Will_Roof(nullptr)
	, m_pObject_Village_BigTree1(nullptr)
	, m_pObject_Village_BigTree2(nullptr)
	, m_pObject_Village_BigTree3(nullptr)
	, m_pObject_Village_Fade_BG(nullptr)
	, m_fTime(0.f)
{
	SetName(L"TownScript");
}

TownScript::~TownScript()
{
}

void TownScript::Start()
{
	m_pObject_Village_House5 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_House4");
	m_pObject_Village_Rival_Fountaion = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_Rival_Fountaion");
	m_pObject_Village_Well = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_Well");
	m_pObject_Village_Will_Roof = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_Will_Roof");
	m_pObject_Village_BigTree1 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_BigTree1");
	m_pObject_Village_BigTree2 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_BigTree2");
	m_pObject_Village_BigTree3 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Village_BigTree3");

	assert(m_pObject_Village_House5);
	assert(m_pObject_Village_Rival_Fountaion);
	assert(m_pObject_Village_Well);
	//assert(m_pObject_Village_Will_Roof);
	assert(m_pObject_Village_BigTree1);
	assert(m_pObject_Village_BigTree2);
	assert(m_pObject_Village_BigTree3);

	m_pObject_Village_House5->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_House4_Anim");
	m_pObject_Village_Rival_Fountaion->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_Rival_Fountaion_Anim");
	m_pObject_Village_Well->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_Well_Anim");
	//m_pObject_Village_Will_Roof->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_Will_Roof_Anim");
	m_pObject_Village_BigTree1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_BigTree_Anim");
	m_pObject_Village_BigTree2->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_BigTree_Anim");
	m_pObject_Village_BigTree3->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Village_BigTree_Anim");

	LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Trigger_DungeonEntrance")->GetScript<TriggerScript>(L"TriggerScript")->SetTriggerType(TRIGGER_TYPE::DUNGEONENTRANCE);

	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TRIGGER);
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::STRUCTURE_WALL_RIGHT);
}

void TownScript::Update()
{
	m_fTime += DT;
	if (m_fTime >= 10.f)
	{
		TimeMgr::GetInst()->SetAccTimeUpdate(TRUE);
	}
}