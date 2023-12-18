#include "pch.h"
#include "EHEngine.h"

#include "EHDevice.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"
#include "EHAssetMgr.h"
#include "EHLevelMgr.h"

#include "EHCamera.h"

extern Camera* MainCamera = new Camera();

Engine::Engine()
	:m_vResolution{}
	, m_hWnd(nullptr)
{

}

Engine::~Engine()
{

}

int Engine::Init(Vec2 _vResolution, HWND _hWnd)
{
	// Window Setting
	m_hWnd = _hWnd;
	m_vResolution = _vResolution;

	RECT rt = { 0,0,(int)m_vResolution.x,(int)m_vResolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	// Device Initialize
	if (FAILED(Device::GetInst()->Init(m_hWnd, m_vResolution)))
	{
		HandleError(m_hWnd, L"Device Initialize Failed!", 1);
		return E_FAIL;
	}

	// Manger Initialize
	TimeMgr::GetInst()->Init();
	PathMgr::GetInst()->Init();
	KeyMgr::GetInst()->Init();
	AssetMgr::GetInst()->Init();
	LevelMgr::GetInst()->Init();

	return S_OK;
}

void Engine::Progress()
{
	// RenderTarget, DepthStencil ÃÊ±âÈ­
	float ClearColor[4] = { 1.f,1.f,1.f,1.f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

	//Manger Progress
	TimeMgr::GetInst()->Progress();
	KeyMgr::GetInst()->Tick();

	LevelMgr::GetInst()->Tick();
	MainCamera->Tick();
	LevelMgr::GetInst()->Render();

	Device::GetInst()->Present();
}
