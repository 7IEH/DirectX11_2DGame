#pragma once

class Engine
	:public Singleton<Engine>
{
	Single(Engine)
private:
	Vec2		m_vResolution;
	HWND		m_hWnd;
	
public:
	HWND		GetMainWind() { return m_hWnd; }

private:
	void		AwakeManager();

public:
	int			Awake(Vec2 _vResolution, HWND _hWnd);
	void		Start();
	void		Update();
};

