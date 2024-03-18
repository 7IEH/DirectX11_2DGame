#pragma once

enum class PLACE
{
	TUTORIAL,
	TOWN,
	None,
};

struct PlayerPref
{
	PLACE	_ePlace;
	Vec2	_vPos;
	int		_iTime;
	int		_bTutorial;
	int		_iClear;
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
	
public:
	void Awake();

public:
	void CreateSaveFile(int _iCurSlot);
	void DeleteSaveFile(int _iCurSlot);
	void SaveFile(int _iCurSlot);
	void LoadFile(int _iCurSlot);



private:
	void WriteFile(int _iCurSlot);

	friend class PlayerScript;
	friend class SlotScript;
};

