#pragma once

class KeyMgr
	:public Singleton<KeyMgr>
{
	Single(KeyMgr);
private:
	vector<FKeyData>	m_vecKeyData;

	Vec2				m_vMousePos;

public:
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeyData[_Key].eState; }
	Vec2 GetMousePos() { return m_vMousePos; }

public:
	void Init();
	void Tick();
};

