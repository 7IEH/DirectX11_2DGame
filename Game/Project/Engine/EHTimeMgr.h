#pragma once
class TimeMgr
	: public Singleton<TimeMgr>
{
	Single(TimeMgr)
private:
	float		m_fDeltaTime;

	LARGE_INTEGER		m_prevCounter;
	LARGE_INTEGER		m_curCounter;
	LARGE_INTEGER		m_Frequency;

	UINT			m_iCall;
	float			m_fTime;
	
	bool			m_bAcctimeUpdate;
	bool			m_bAcctimeSet;
	
	bool			m_bFlag;

public:
	float				GetDeltaTime() { return m_fDeltaTime; }
	
	void				SetAccTimeUpdate(bool _bAccTimeUpdtae) { m_bAcctimeUpdate = _bAccTimeUpdtae; }
	void				SetAccTimeSet(bool _bAcctimeSet) { m_bAcctimeSet = _bAcctimeSet; }

public:
	void				Awake();
	void				Update();
};

