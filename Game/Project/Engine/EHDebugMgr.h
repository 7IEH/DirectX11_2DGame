#pragma once

class DebugMgr
	:public Singleton<DebugMgr>
{
	Single(DebugMgr);
private:
	vector<string> m_DebugMessage;

public:
	vector<string>	GetDebugMessage() { return m_DebugMessage; }
	void Log(const string _message) { m_DebugMessage.push_back(_message); };

public:
	void LateUpdate();
};

