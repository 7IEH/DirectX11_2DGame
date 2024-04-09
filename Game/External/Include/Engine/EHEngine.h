#pragma once

class Engine
	:public Singleton<Engine>
{
	Single(Engine)
private:
	Vec2		m_vResolution;
	HWND		m_hWnd;

	/*winrt::com_ptr<media::MediaEngineWrapper> mediaEngineWrapper;
	media::MFPlatformRef mfPlatform;

	winrt::com_ptr<IDXGIDevice> _pDxgiDevice;
	winrt::com_ptr<IDCompositionDevice> _pCompDevice;
	winrt::com_ptr<IDCompositionDevice2> _pCompDevice2;
	winrt::com_ptr<IDCompositionVisual2> _prootVisual;
	winrt::com_ptr<IDCompositionTarget> _pComTarget;*/

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

