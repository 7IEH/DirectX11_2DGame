#include "pch.h"
#include "EHEngine.h"

#include <random>

#include "EHDevice.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"
#include "EHAssetMgr.h"
#include "EHLevelMgr.h"
#include "EHTaskMgr.h"
#include "EHThreadMgr.h"
#include "EHRenderMgr.h"	
#include "EHImGUIMgr.h"
#include "EHCollisionMgr.h"
#include "EHGarbageCollector.h"
#include "EHDebugMgr.h"
#include "EHAnimationMgr.h"
#include <EHScriptMgr.h>
#include <EHRecordManager.h>
#include <EHRandomManager.h>
#include <EHRoomManager.h>
#include "EHFontMgr.h"

#include "EHCamera.h"

#include "EHHierarchy.h"

Engine::Engine()
	: m_vResolution{}
	, m_hWnd(nullptr)
{

}

Engine::~Engine()
{
}

int Engine::Awake(Vec2 _vResolution, HWND _hWnd)
{
	// Window Setting
	m_hWnd = _hWnd;
	m_vResolution = _vResolution;

	RECT rt = { 0,0,(int)m_vResolution.x,(int)m_vResolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	// Device Initialize
	if (FAILED(Device::GetInst()->Awake(m_hWnd, m_vResolution)))
	{
		HandleError(m_hWnd, L"Device Initialize Failed!", 1);
		return E_FAIL;
	}

	// Manger Initialize
	AwakeManager();

	Start();

	return S_OK;
}

void Engine::ChangeResoultion(Vec2 _resolution)
{
	m_vResolution = _resolution;

	RECT rt = { 0,0,(int)m_vResolution.x,(int)m_vResolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void Engine::AwakeManager()
{
	TimeMgr::GetInst()->Awake();
	PathMgr::GetInst()->Awake();
	KeyMgr::GetInst()->Awake();
	AssetMgr::GetInst()->Awake();
	AnimationMgr::GetInst()->Awake();
	ScriptMgr::GetInst()->Awake();
	LevelMgr::GetInst()->Awake();
	RenderMgr::GetInst()->Awake();
	FontMgr::GetInst()->Awake();
	RecordManager::GetInst()->Awake();
	RandomManager::GetInst()->Awake();
	RoomManager::GetInst()->Awake();

#ifdef _DEBUG
	LevelMgr::GetInst()->SelectLevel(L"GolemDungeonBossScene");
	if (FALSE == ImGUIMgr::GetInst()->Awake())
	{
		HandleError(MAIN_HWND, L"ImGUI InitailizeError", 1);
	}
#else
	LevelMgr::GetInst()->SelectLevel(L"GolemDungeonBossScene");
#endif

#ifdef _DEBUG
	static_cast<Hierarchy*>(ImGUIMgr::GetInst()->FindUI("Hierarchy"))->ResetCurrentLevel();
#endif
}

void Engine::Start()
{
	//MediaEnginePlaybackSample();
}

void Engine::Update()
{
	//	Manger Progress
	TimeMgr::GetInst()->Update();
	KeyMgr::GetInst()->Update();

	// Level Update
	LevelMgr::GetInst()->Update();
	CollisionMgr::GetInst()->Update();

	if (RenderMgr::GetInst()->GetRender())
		RenderMgr::GetInst()->Update();

	DebugMgr::GetInst()->LateUpdate();

	if (KEY_TAP(KEY::SPACE))
	{
		ThreadMgr::GetInst()->StartThread(L"MapGenerator1");
	}

	// GarbageCollector
	// GameObject -> Memory Release
	GarbageCollector::GetInst()->Update();

	// Task Update
	TaskMgr::GetInst()->Update();
}

//void Engine::MediaEnginePlaybackSample()
//{
////	// Ensure that MediaFoundation has been started up using the MFPlatformRef helper object
////	mfPlatform.Startup();
////
////	// Create a source resolver to create an IMFMediaSource for the content URL.
////	// This will create an instance of an inbuilt OS media source for playback.
////	// An application can skip this step and instantiate a custom IMFMediaSource implementation instead.
////	winrt::com_ptr<IMFSourceResolver> sourceResolver;
////	THROW_IF_FAILED(MFCreateSourceResolver(sourceResolver.put()));
////	constexpr uint32_t sourceResolutionFlags = MF_RESOLUTION_MEDIASOURCE | MF_RESOLUTION_READ;
////	MF_OBJECT_TYPE objectType = {};
////	winrt::com_ptr<IMFMediaSource> mediaSource;
////	THROW_IF_FAILED(sourceResolver->CreateObjectFromURL(c_testContentURL, sourceResolutionFlags, nullptr, &objectType, reinterpret_cast<IUnknown**>(mediaSource.put_void())));
////
////	// Lambdas to handle callbacks
////
////	// DirectCompositionWindow callback to inform app of size changes
////	auto onWindowSizeChanged = [&](uint32_t width, uint32_t height) {
////		// If the MediaEngineWrapper has been created, notify it of window size changes so that it can update the video surface size.
////		if (mediaEngineWrapper)
////		{
////			mediaEngineWrapper->OnWindowUpdate(width, height);
////		}
////	};
////
////	// MediaEngineWrapper initialization callback which is invoked once the media has been loaded and a DCOMP surface handle is available
////	auto onInitialized = [&]() {
////		// Create video visual and add it to the DCOMP tree
////		winrt::com_ptr<IDCompositionDevice> dcompDevice;
////		THROW_IF_FAILED(_pCompDevice2->QueryInterface(IID_PPV_ARGS(dcompDevice.put())));
////		std::shared_ptr<ui::DirectCompositionLayer> videoLayer =
////			ui::DirectCompositionLayer::CreateFromSurface(dcompDevice.get(), mediaEngineWrapper->GetSurfaceHandle());
////		winrt::com_ptr<IDCompositionVisual2> rootVisual;
////		rootVisual.copy_from(_prootVisual.get());
////		THROW_IF_FAILED(rootVisual->AddVisual(videoLayer->GetVisual(), TRUE, nullptr));
////		_pCompDevice2->Commit();
////
////		// Start playback
////		/*mediaEngineWrapper->StartPlayingFrom(0);*/
////	};
////
////	auto onError = [&](MF_MEDIA_ENGINE_ERR error, HRESULT hr)
////	{
////		wchar_t message[100] = {};
////		wsprintf(message, L"Playback failed. error=%d, hr=0x%x", error, hr);
////		MessageBox(GetDesktopWindow(), message, nullptr, MB_OK | MB_ICONEXCLAMATION);
////	};
////
////
////#pragma region Initialize_Window
////	DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)&_pDxgiDevice);
////	THROW_IF_FAILED(DCompositionCreateDevice2(_pDxgiDevice.get(), IID_PPV_ARGS(_pCompDevice.put())));
////	_pCompDevice2 = _pCompDevice.as<IDCompositionDevice2>();
////
////	// Create target against HWND
////	winrt::com_ptr<IDCompositionDesktopDevice> desktopDevice = _pCompDevice2.as < IDCompositionDesktopDevice>();
////	desktopDevice->CreateTargetForHwnd(m_hWnd, TRUE, _pComTarget.put());
////	// Create root visual and set it on the target
////	_pCompDevice2->CreateVisual(_prootVisual.put());
////	_pComTarget->SetRoot(_prootVisual.get());
////
////	// Create and initialize the MediaEngineWrapper which manages media playback
////#pragma endregion
////
////	mediaEngineWrapper = winrt::make_self<media::MediaEngineWrapper>(onInitialized, onError, nullptr, nullptr, nullptr);
////	mediaEngineWrapper->Initialize(mediaSource.get());
////
////	mediaEngineWrapper->StartPlayingFrom(0);
////
////	//Keep thread alive until window is closed
//}