#pragma once
class TimeMgr
	: public Singleton<TimeMgr>
{
	Single(TimeMgr)
private:
	static float		m_fDeltaTime;

	LARGE_INTEGER		m_prevCounter;
	LARGE_INTEGER		m_curCounter;
	LARGE_INTEGER		m_Frequency;

public:
	float				GetDeltaTime() { return m_fDeltaTime; }

public:
	void				Init();
	void				Progress();
};

