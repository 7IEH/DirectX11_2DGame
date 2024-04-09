#include "pch.h"
#include "EHVideoMgr.h"

VideoMgr::VideoMgr()
	: m_nRefCount(0)
	, m_pSource(nullptr)
	, m_hwndVideo(nullptr)
	, m_hwndEvent(nullptr)
	, m_state(PlayerState::Closed)
	, m_hCloseEvent(nullptr)
{
}

VideoMgr::~VideoMgr()
{
}

void VideoMgr::Awake()
{
	HRESULT _hr = Initialize();
	MFCreateSourceResolver(&m_pResolver);
}

void VideoMgr::Update()
{
}

HRESULT VideoMgr::OpenURL(const wstring& _path)
{
	constexpr uint32_t sourceResolutionFlags = MF_RESOLUTION_MEDIASOURCE | MF_RESOLUTION_READ;
	MF_OBJECT_TYPE _eObjectType = {};
	m_pResolver->CreateObjectFromURL(_path.c_str(), sourceResolutionFlags, nullptr, &_eObjectType, reinterpret_cast<IUnknown**>(&m_pSource));
	


	return E_NOTIMPL;
}

HRESULT VideoMgr::Play()
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::Pause()
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::Stop()
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::Shutdown()
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::HandleEvent(UINT_PTR pUnkPtr)
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::Repaint()
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::ResizeVideo(WORD width, WORD height)
{
	return E_NOTIMPL;
}

HRESULT VideoMgr::Initialize()
{
	HRESULT _hr = MFStartup(MF_VERSION, MFSTARTUP_FULL);

	if (SUCCEEDED(_hr))
	{
		m_hCloseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hCloseEvent == nullptr)
		{
			_hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}
	return _hr;
}

HRESULT VideoMgr::CloseSession()
{
	return E_NOTIMPL;
}