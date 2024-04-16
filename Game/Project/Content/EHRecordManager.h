#pragma once

#include "EHItemScript.h"

enum class PLACE
{
	TUTORIAL,
	TOWN,
	DUNGEONENTRANCE,
	None,
};

enum class Weapon
{
	BasicOneHand,
	BasicTwoHand,
	BroomStick,
	None,
};

enum class SubWeapon
{
	BasicShield,
	None,
};

struct PlayerPref
{
	Vec3	_vSaveDay;
	PLACE	_ePlace;
	Vec2	_vPos;
	int		_iTime;
	int		_bTutorial;
	int		_iClear;

	// 플레이어 정보
	// 추가 할 때마다 실행코드도 고치기!
	int			_iCurHp;
	UINT		_iMaxHp;
	int			_iMoney;
	UINT		_iStrikingPower;
	float		_fSpeed;
	int			_iCurWeapon;
	ITEM		_iWeapon1;
	ITEM		_iWeapon2;
	ITEM		_iUseItem;

	// 인벤토리
	ITEM		_eInventory[20];
	int			_iInventory[20];
};

class RecordManager
	:public Singleton<RecordManager>
{
	Single(RecordManager)

private:
	wstring				m_wSavePath;
	vector<PlayerPref*> m_vPlayerPref;

	int					m_iCurSlot;

private:
	PlayerPref* GetCurrentPlayerPref() { return m_vPlayerPref[m_iCurSlot]; }
	void SetCurrentPlayerPref(PlayerPref* _pPref)  { m_vPlayerPref[m_iCurSlot] = _pPref; }

	const vector<PlayerPref*>& GetPlayerPref() { return m_vPlayerPref; }

public:
	void Awake();

public:
	void CreateSaveFile(int _iCurSlot);
	void DeleteSaveFile(int _iCurSlot);
	void SaveFile();
	void LoadFile(int _iCurSlot);

	bool IsSavedFile();

private:
	void WriteFile(int _iCurSlot);

	friend class TitleScript;
	friend class PlayerScript;
	friend class TutorialScript;
	friend class SlotScript;
	friend class PUIScript;
	friend class EnemyScript;
};

