#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <evr.h>

enum PlayerState
{
	Closed = 0,
	Ready,
	OpenPending,
	Started,
	Paused,
	Stopped,
	Closing
};

class VideoMgr
	:public Singleton<VideoMgr>
{
	Single(VideoMgr)
protected:
	long                    m_nRefCount;        // Reference count.


	IMFSourceResolver*		m_pResolver;
	IMFMediaSource*			m_pSource;
	

	HWND                    m_hwndVideo;        // Video window.
	HWND                    m_hwndEvent;        // App window to receive events.
	PlayerState             m_state;            // Current state of the media session.
	HANDLE                  m_hCloseEvent;

public:
	HRESULT       OpenURL(const wstring& _path);
	HRESULT       Play();
	HRESULT       Pause();
	HRESULT       Stop();
	HRESULT       Shutdown();
	HRESULT       HandleEvent(UINT_PTR pUnkPtr);
	PlayerState   GetState() const { return m_state; }

	// Video functionality
	HRESULT       Repaint();
	HRESULT       ResizeVideo(WORD width, WORD height);

private:
	HRESULT Initialize();
	HRESULT CreateSession();
	HRESULT CloseSession();

public:
	void Awake();
	void Update();
};