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

void LevelMgr::Awake()
{
	string _path = EH::ConvertString(PATH) + "\\Assets\\Scenes";

	for (const std::filesystem::directory_entry& _entry :
		std::filesystem::directory_iterator(_path))
	{
		wstring _path = _entry.path().native();
		wstring _levelName = _entry.path().native();
		_levelName = _levelName.substr(_levelName.find_last_of('\\') + 1,
			_levelName.find_last_of('.') - _levelName.find_last_of('\\') - 1);
		Level* _level = AddLevel<Level>(_levelName);
		_level->Load(string(_path.begin(), _path.end()));
	}

	TestLevel* _level = AddLevel<TestLevel>(L"TestLevel");
	AddLevel<IntroLevel>(L"IntroLevel");
	AddLevel<TitleLevel>(L"TitleLevel");
	AddLevel<DungeonScene>(L"DungeonScene");
	SelectLevel(L"TestLevel");

	m_CurLevel = _level->Clone();
	m_CurLevel->Awake();

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
	Save();
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
				std::ofstream _fileInfo(_entry.path().native().data());
				// file ��� ����
				_fileInfo.clear();

				// ���� ����
				// �� GameObject �Լ� ȣ��
				// layerMatrix

				_fileInfo << "CollisionMatrix\n";

				UINT* _collisionMatrix = CollisionMgr::GetInst()->GetCollisionMatrix();
				for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
				{	
					_fileInfo << std::to_string(_collisionMatrix[i]) + '\n';
				}

				_fileInfo.close();

				for (UINT _idx = 0;_idx < (UINT)LAYER_TYPE::END;_idx++)
				{
					Layer* _layer = m_CurLevel->GetLayer(LAYER_TYPE(_idx));
					if (_layer->GetLayerParent().size() != 0)
					{
						// Save Layer �ϱ�
						_layer->Save(string(_entry.path().native().begin(), _entry.path().native().end()));
					}
				}
			}
		}
		// ANIMATION SAVE
		AnimationMgr::GetInst()->Save();
	}
}