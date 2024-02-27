#include "pch.h"
#include "EHEngine.h"

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
#include "EHFontMgr.h"

#include "EHCamera.h"

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

	if (FALSE == ImGUIMgr::GetInst()->Awake())
	{
		HandleError(MAIN_HWND, L"ImGUI InitailizeError", 1);
	}

#ifdef _DEBUG
	LevelMgr::GetInst()->SelectLevel(L"EnterScene");
#else
	LevelMgr::GetInst()->SelectLevel(L"EnterScene");
#endif
}

void Engine::Start()
{
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
