#include "pch.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

#include "EHTestLevel.h"
#include <EHDungeonScene.h>
#include <EHTitleScene.h>
#include <EHIntroScene.h>

#include "EHDebugMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"
#include "EHAnimationMgr.h"
#include "EHCollisionMgr.h"

LevelMgr::LevelMgr()
	:m_CurLevel(nullptr)
{

}

LevelMgr::~LevelMgr()
{
	for (auto nxt : m_Levels)
	{
		if (nxt.second != nullptr)
			delete nxt.second;
	}
}

void LevelMgr::AddLevel(wstring _levelName, Level* _level)
{
	map<wstring, Level*>::iterator iter = m_Levels.find(_levelName);
	_level->SetName(_levelName);
	if (m_Levels.end() != iter)
	{
		HandleError(MAIN_HWND, L"LevelMgr AddLevelFunc Duplication Error!", 2);
		return;
	}
	m_Levels.insert(make_pair(_levelName, _level));

}

void LevelMgr::Awake()
{
	string _path = EH::ConvertString(PATH) + "\\Assets\\Scenes";

	for (const std::filesystem::directory_entry& _entry :
		std::filesystem::directory_iterator(_path))
	{
		wstring _levelName = _entry.path().native().c_str();
		_levelName = _levelName.substr(_levelName.find_last_of('\\') + 1,
			_levelName.find_last_of('.') - _levelName.find_last_of('\\') - 1);
		Level* _level = AddLevel<Level>(_levelName);
		_level->Load(string(_entry.path().native().begin(), _entry.path().native().end()));
	}
	AddLevel<IntroLevel>(L"IntroScene");
	if (m_CurLevel == nullptr)
		return;
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

	// �̰� ���߿� UI ������������ �ϰ� �����

#ifdef _DEBUG
	Save();
#endif
}

void LevelMgr::Save()
{
	// ���� ������ ���� ��� �������� �ʰ� ��
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		wstring _wcurName = m_CurLevel->GetName();
		string _curName = string(_wcurName.begin(), _wcurName.end());
		wstring _absolutPath = PATH;
		string _path = EH::ConvertString(_absolutPath.c_str()) + "\\Assets\\Scenes";

		for (const std::filesystem::directory_entry& _entry :
			std::filesystem::directory_iterator(_path))
		{
			string _fileName = string(_entry.path().native().begin(),
				_entry.path().native().end());

			_fileName = _fileName.substr(_fileName.find_last_of('\\') + 1, _fileName.find_last_of('.') - _fileName.find_last_of('\\') - 1);

			// scene�� ���� ������ �����غ��� �ʿ����
			// scene�� �̹� ������ �Ŀ� ����� ����
			if (_curName == _fileName)
			{
				std::wofstream _fileInfo(_entry.path().native().data());
				// file ��� ����
				_fileInfo.clear();

				// ���� ����
				// �� GameObject �Լ� ȣ��
				// layerMatrix

				_fileInfo << "CollisionMatrix\n";

				UINT* _collisionMatrix = CollisionMgr::GetInst()->GetCollisionMatrix();
				for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
				{	
					_fileInfo << std::to_wstring(_collisionMatrix[i]) + L'\n';
				}

				for (UINT _idx = 0;_idx < (UINT)LAYER_TYPE::END;_idx++)
				{
					Layer* _layer = m_CurLevel->GetLayer(LAYER_TYPE(_idx));
					if (_layer->GetLayerParent().size() != 0)
					{
						// Save Layer �ϱ�
						_layer->Save(&_fileInfo);
					}
				}
				_fileInfo.close();
			}
		}
		// ANIMATION SAVE
		AnimationMgr::GetInst()->Save();
	}
}