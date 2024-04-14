#include "pch.h"
#include "EHDungeonEntranceScript.h"

#include <EHLevelMgr.h>

#include "EHTriggerScript.h"

DungeonEntranceScript::DungeonEntranceScript()
	:m_pTownTrigger(nullptr)
	, m_pDungeonTrigger(nullptr)
{
	SetName(L"DungeonEntranceScript");
}

DungeonEntranceScript::~DungeonEntranceScript()
{
}

void DungeonEntranceScript::Start()
{
	FIND_OBJECT(L"MainLight")->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));

	m_pTownTrigger = FIND_OBJECT(L"Object_Trigger_Town");
	m_pDungeonTrigger = FIND_OBJECT(L"Object_Trigger_GolemDungeon");

	assert(m_pTownTrigger);
	assert(m_pDungeonTrigger);

	m_pTownTrigger->GetScript<TriggerScript>()->SetTriggerType(TRIGGER_TYPE::TOWN);
	m_pDungeonTrigger->GetScript<TriggerScript>()->SetTriggerType(TRIGGER_TYPE::GOLEM_DUNGEON);

	LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera")->GetScript<CameraTargetScript>()->
		SetTarget(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player"));

	Object::Play2DBGM(L"\\resource\\Audio\\dungeon_entrance_wind_ambient.wav", 0.5f);
	Object::Play2DSound(L"\\resource\\Audio\\dungeon_entrance_fabric.wav", FALSE, 0.2f);
}

void DungeonEntranceScript::Update()
{
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TRIGGER);
}