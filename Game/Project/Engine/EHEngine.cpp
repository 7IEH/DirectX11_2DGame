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
#include <EHItemMgr.h>

#include "EHCamera.h"

#include "EHHierarchy.h"
#include "EHSound.h"

Engine::Engine()
	: m_vResolution{}
	, m_hWnd(nullptr)
{

}

Engine::~Engine()
{
	if (nullptr != Sound::g_pFMOD)
	{
		Sound::g_pFMOD->release();
		Sound::g_pFMOD = nullptr;
	}
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
	ItemMgr::GetInst()->Awake();
	RandomManager::GetInst()->Awake();
	RoomManager::GetInst()->Awake();
	RecordManager::GetInst()->Awake();

#ifdef _DEBUG
	LevelMgr::GetInst()->SelectLevel(L"EnterScene");
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

	// FMOD Update
	Sound::g_pFMOD->update();

	DebugMgr::GetInst()->LateUpdate();

	// GarbageCollector
	// GameObject -> Memory Release
	GarbageCollector::GetInst()->Update();

	// Task Update
	TaskMgr::GetInst()->Update();
}