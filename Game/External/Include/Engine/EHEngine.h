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

public:
	int			Init(Vec2 _vResolution, HWND _hWnd);
	void		Progress();
};

