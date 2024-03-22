#include "pch.h"
#include "EHRecordManager.h"

#include <EHPathMgr.h>

RecordManager::RecordManager()
	: m_wSavePath{}
	, m_vPlayerPref{}
	, m_iCurSlot(0)
{
	m_wSavePath = PATH;
	m_wSavePath += L"\\saved\\";
	m_vPlayerPref.resize(5);
}

RecordManager::~RecordManager()
{
	ReleaseVector(m_vPlayerPref);
}

void RecordManager::Awake()
{
	for (int i = 0;i < 5;i++)
	{
		if (std::filesystem::exists(m_wSavePath + std::to_wstring(i)))
		{
			m_vPlayerPref[i] = new PlayerPref;
		}
	}
}

void RecordManager::CreateSaveFile(int _iCurSlot)
{
	// 해당 파일이 있으면
	//해당 파일이 없으면
	// 1. 현재 파일 목록 읽어오기
	// 2. 현재 부른 곳에 savefile 미 존재시 만듬
	// 3. 있으면 load
	if (std::filesystem::exists(m_wSavePath + std::to_wstring(_iCurSlot)))
	{
		LoadFile(_iCurSlot);
	}
	else
	{
		std::filesystem::create_directory(m_wSavePath + std::to_wstring(_iCurSlot));
		std::ofstream _pFile(m_wSavePath + std::to_wstring(_iCurSlot) + L"\\Saved.save",
			std::ios::out | std::ios::app);
		_pFile.close();

		m_vPlayerPref[_iCurSlot] = new PlayerPref;

		m_vPlayerPref[_iCurSlot]->_bTutorial = 0;
		m_vPlayerPref[_iCurSlot]->_ePlace = PLACE::TUTORIAL;
		m_vPlayerPref[_iCurSlot]->_iTime = 0;
		m_vPlayerPref[_iCurSlot]->_vPos = Vec2(0.f, 0.f);
		m_vPlayerPref[_iCurSlot]->_iClear = 0;
		m_vPlayerPref[_iCurSlot]->_iCurHp = 100;
		m_vPlayerPref[_iCurSlot]->_iMaxHp = 100;
		m_vPlayerPref[_iCurSlot]->_iMoney = 0;
		m_vPlayerPref[_iCurSlot]->_fSpeed = 300.f;
		m_vPlayerPref[_iCurSlot]->_iStrikingPower = 10;
		m_vPlayerPref[_iCurSlot]->_eWeapon = Weapon::Spear;
		m_vPlayerPref[_iCurSlot]->_eSubWeapon = SubWeapon::None;

		m_iCurSlot = _iCurSlot;
		WriteFile(_iCurSlot);
	}
}

void RecordManager::DeleteSaveFile(int _iCurSlot)
{
	if (std::filesystem::exists(m_wSavePath + std::to_wstring(_iCurSlot)))
	{
		std::filesystem::remove(m_wSavePath + std::to_wstring(_iCurSlot));
	}
}

void RecordManager::SaveFile(int _iCurSlot)
{
	// 튜토리얼 유무
	// 현재 Scene
	// 현재 Position
	// 현재 시각
	// 클리어 던전
	// 플레이어 정보
	std::ofstream _pFile(m_wSavePath + std::to_wstring(_iCurSlot) + L"\\Saved.save",
		std::ios::out | std::ios::app);

	WriteFile(_iCurSlot);
}

void RecordManager::LoadFile(int _iCurSlot)
{
	// 튜토리얼 유무
	// 현재 Scene
	// 현재 Position
	// 현재 시각
	// 클리어 던전
	// 플레이어 정보
	std::ifstream _pFile(m_wSavePath + std::to_wstring(_iCurSlot) + L"\\Saved.save");

	m_iCurSlot = _iCurSlot;

	string _line = {};
	int _idx = 0;
	while (std::getline(_pFile, _line))
	{
		if (0 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_bTutorial = std::stoi(_line);
		}
		else if (1 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_ePlace = PLACE(std::stoi(_line));
		}
		else if (2 == _idx)
		{
			EH::InputVector2(_line, m_vPlayerPref[_iCurSlot]->_vPos);
		}
		else if (3 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iTime = std::stoi(_line);
		}
		else if (4 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iClear = std::stoi(_line);
		}
		else if (5 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iCurHp = std::stoi(_line);
		}
		else if (6 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iMaxHp = std::stoi(_line);
		}
		else if (7 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iMoney = std::stoi(_line);
		}
		else if (8 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_fSpeed = std::stof(_line);
		}
		else if (9 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_iStrikingPower = std::stoi(_line);
		}
		else if (10 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_eWeapon = Weapon(std::stoi(_line));
		}
		else if (11 == _idx)
		{
			m_vPlayerPref[_iCurSlot]->_eSubWeapon = SubWeapon(std::stoi(_line));
		}
		_idx++;
	}

	_pFile.close();
}

void RecordManager::WriteFile(int _iCurSlot)
{
	std::ofstream _pFile(m_wSavePath + std::to_wstring(_iCurSlot) + L"\\Saved.save",
		std::ios::out | std::ios::app);

	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_bTutorial) + '\n';
	_pFile << std::to_string(int(m_vPlayerPref[_iCurSlot]->_ePlace)) + '\n';
	_pFile << EH::WriteVector2(m_vPlayerPref[_iCurSlot]->_vPos) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iTime) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iClear) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iCurHp) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iMaxHp) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iMoney) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_fSpeed) + '\n';
	_pFile << std::to_string(m_vPlayerPref[_iCurSlot]->_iStrikingPower) + '\n';
	_pFile << std::to_string(int(m_vPlayerPref[_iCurSlot]->_eWeapon)) + '\n';
	_pFile << std::to_string(int(m_vPlayerPref[_iCurSlot]->_eSubWeapon)) + '\n';

	_pFile.close();
}