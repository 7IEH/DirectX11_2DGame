#pragma once

#include "EHDevice.h"
#include "EHLevel.h"

class LevelMgr
	:public Singleton<LevelMgr>
{
	Single(LevelMgr);
private:
	map<wstring, Level*> m_Levels;
	Level* m_CurLevel;

public:
	template <typename T>
	void AddLevel(wstring _levelName)
	{
		T* _level = new T();
		map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);
		if (iter != m_Levels.end())
		{
			HandleError(MAIN_HWND, L"LevelMgr AddLevelFunc Duplication Error!", 2);
			return;
		}
		m_Levels.insert(make_pair(_levelName, _level));
	}

	void SelectLevel(wstring _levelName)
	{
		map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);
		if (iter == m_Levels.end())
		{
			HandleError(MAIN_HWND, L"LevelMgr FindFunc Not Found!", 2);
			return;
		}
		m_CurLevel = (*iter).second;
		m_CurLevel->Awake();
	}

	void SelectLevel(Level* _curLevel)
	{
		m_CurLevel = _curLevel;
		m_CurLevel->Awake();
	}

	Level* FindLevel(const wstring& _levelName)
	{
		map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);

		if (iter == m_Levels.end())
		{
			return nullptr;
		}

		return iter->second;
	}

public:
	Level* GetCurLevel() { return m_CurLevel; }

public:
	void Awake();
	void Start();
	void Update();
};
