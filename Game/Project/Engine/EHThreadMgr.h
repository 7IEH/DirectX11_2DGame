#pragma once

class ThreadMgr
	:public Singleton<ThreadMgr>
{
	Single(ThreadMgr)
private:
	vector<std::thread> m_ThreadTask;
	map<wstring, HANDLE> m_EventPoint;

public:
	void Awake();
	void StartThread(const wstring& _name);

private:
	HANDLE FindThread(const wstring& _name);
};

