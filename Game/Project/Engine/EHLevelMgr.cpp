#include "pch.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

#include "EHTestLevel.h"

LevelMgr::LevelMgr()
	:m_CurLevel(nullptr)
	,m_Levels{}
{

}

LevelMgr::~LevelMgr()
{

}

void LevelMgr::Init()
{
	AddLevel<TestLevel>(L"TestLevel");
	SelectLevel(L"TestLevel");
	m_CurLevel->Init();
}

void LevelMgr::Tick()
{
	m_CurLevel->Tick();
	m_CurLevel->FinalTick();
}

void LevelMgr::Render()
{
	m_CurLevel->Render();
}
