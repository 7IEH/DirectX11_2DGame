#pragma once

#include "EHDevice.h"
#include "EHLevel.h"
#include "EHCollisionMgr.h"

class LevelMgr
	:public Singleton<LevelMgr>
{
	Single(LevelMgr);
private:
	map<wstring, Level*> m_Levels;
	Level* m_CurLevel;
public:
	template <typename T>
	T* AddLevel(wstring _levelName)
	{
		T* _level = new T();
		_level->SetName(_levelName);
		map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);
		if (iter != m_Levels.end())
		{
			HandleError(MAIN_HWND, L"LevelMgr AddLevelFunc Duplication Error!", 2);
			return nullptr;
		}
		m_Levels.insert(make_pair(_levelName, _level));

		return _level;
	}

	void AddLevel(wstring _levelName, Level* _level);

	void SelectLevel(wstring _levelName)
	{
		map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);
		CollisionMgr::GetInst()->Clear();
		if (iter == m_Levels.end())
		{
			HandleError(MAIN_HWND, L"LevelMgr FindFunc Not Found!", 2);
			return;
		}
		m_CurLevel = (*iter).second;
		m_CurLevel->Awake();
	}

	void DeletLevel(wstring _levelName)
	{
		Level* _level = FindLevel(_levelName);

		if (_level == nullptr)
			return;

		m_Levels.erase(_levelName);
		delete _level;
		_level = nullptr;
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

private:
	void Save();
};
