#include "pch.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

#include "EHTestLevel.h"
#include <EHDungeonScene.h>
#include <EHTitleScene.h>
#include <EHIntroScene.h>
#include "EHDebugMgr.h"
#include "EHPathMgr.h"

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
	string _path = EH::ConvertString(PATH) + "\\Assets\\Scenes";

	for (const std::filesystem::directory_entry& _entry :
		std::filesystem::directory_iterator(_path))
	{
		wstring _path = _entry.path().native();
		wstring _levelName = _entry.path().native();
		_levelName = _levelName.substr(_levelName.find_last_of('\\') + 1,
			_levelName.find_last_of('.') - _levelName.find_last_of('\\') -1);
		Level* _level = AddLevel<Level>(_levelName);
		_level->Initial_Setting(string(_path.begin(),_path.end()));
	}
	
	AddLevel<TestLevel>(L"TestLevel");
	AddLevel<IntroLevel>(L"IntroLevel");
	AddLevel<TitleLevel>(L"TitleLevel");
	AddLevel<DungeonScene>(L"DungeonScene");
	SelectLevel(L"TestLevel");

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