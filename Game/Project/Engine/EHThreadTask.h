#pragma once
class ThreadTask
{
private:
	HANDLE		m_Trigger;

public:
	virtual void Awake();
	virtual void Update() {};

	void SetTrigger(HANDLE _trigger) { m_Trigger = _trigger; }
	HANDLE GetTrigger() { return m_Trigger; }

public:
	ThreadTask();
	virtual ~ThreadTask();
};

