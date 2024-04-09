#include "pch.h"
#include "EHDungeonScript.h"

#include "EHRoomManager.h"

#include <EHLevelMgr.h>

DungeonScript::DungeonScript()
	: m_iCurType{}
	, m_pMainCam(nullptr)
{
	SetName(L"DungeonScript");
}

DungeonScript::~DungeonScript()
{
}

void DungeonScript::Start()
{
	m_pMainCam = FIND_OBJECT(L"MainCamera");
	RoomManager::GetInst()->Start(m_iCurType);

	m_pMainCam->GetComponent<Camera>(COMPONENT_TYPE::CAMERA)->SetFar(20000.f);
}

void DungeonScript::Update()
{
	RoomManager::GetInst()->Update(m_iCurType);
}

void DungeonScript::LateUpdate()
{
}