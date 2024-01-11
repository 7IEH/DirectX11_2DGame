#include "pch.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

#include "EHTestLevel.h"
#include <EHDungeonScene.h>
#include <EHTitleScene.h>
#include <EHIntroScene.h>

LevelMgr::LevelMgr()
	:m_CurLevel(nullptr)
{

}

LevelMgr::~LevelMgr()
{
	for (auto nxt : m_Levels)
	{
		if(nxt.second!=nullptr)
			delete nxt.second;
	}
}

void LevelMgr::Awake()
{
	AddLevel<TestLevel>(L"TestLevel");
	AddLevel<IntroLevel>(L"IntroLevel");
	AddLevel<TitleLevel>(L"TitleLevel");
	AddLevel<DungeonScene>(L"DungeonScene");
	SelectLevel(L"TitleLevel");

	if (m_CurLevel == nullptr)
		return;

	//m_CurLevel->Awake();
}

void LevelMgr::Start()
{
	if (m_CurLevel == nullptr)
		return;

	m_CurLevel->Start();
}

void LevelMgr::Update()
{
	if (m_CurLevel == nullptr)
		return;

	m_CurLevel->Clear();

	m_CurLevel->Update();
	m_CurLevel->LateUpdate();
}