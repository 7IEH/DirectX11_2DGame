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

	Vec2		GetResolution() { return m_vResolution; }
public:
	void		ChangeResoultion(Vec2	_resolution);

	//void		MediaEnginePlaybackSample();
private:
	void		AwakeManager();

public:
	int			Awake(Vec2 _vResolution, HWND _hWnd);
	void		Start();
	void		Update();
};

