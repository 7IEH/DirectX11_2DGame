#pragma once
class ImGUIMgr
	:public Singleton<ImGUIMgr>
{
	Single(ImGUIMgr)
private:
	bool m_Enabled;


public:
	void Enabled() { m_Enabled = !m_Enabled; }
	bool IsEnabled() { return m_Enabled; }

public:
	HRESULT Awake();
	void Render();

private:
	void Frame();
};

