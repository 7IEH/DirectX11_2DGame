#include "pch.h"
#include "EHEngine.h"

#include "EHDevice.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"

#include "EHTest.h"

Test* test = new Test;

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
	SetWindowPos(m_hWnd, nullptr, 10.f, 10.f, rt.right - rt.left, rt.bottom - rt.top, 0);
	
	// Manger Initialize
	TimeMgr::GetInst()->Init();
	PathMgr::GetInst()->Init();
	KeyMgr::GetInst()->Init();

	// Device Initialize
	if (FAILED(Device::GetInst()->Init(m_hWnd, m_vResolution)))
	{
		HandleError(m_hWnd, L"Device Initialize Failed!", 1);
		return E_FAIL;
	}

	// Test Scene
	test->Init(_hWnd);

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

	test->Tick();
	test->Render();

	Device::GetInst()->Present();
}
