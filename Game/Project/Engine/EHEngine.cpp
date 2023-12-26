#include "pch.h"
#include "EHEngine.h"

#include "EHDevice.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"
#include "EHAssetMgr.h"
#include "EHLevelMgr.h"
#include "EHTaskMgr.h"

#include "EHCamera.h"

Engine::Engine()
	:m_vResolution{}
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
	LevelMgr::GetInst()->Awake();
}

void Engine::Start()
{
	// Script 초기화 후 Start 시작
}

void Engine::Update()
{
	// RenderTarget, DepthStencil 초기화
	float ClearColor[4] = { 1.f,1.f,1.f,1.f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

	//	Manger Progress
	TimeMgr::GetInst()->Update();
	KeyMgr::GetInst()->Update();

	// Level Update
	LevelMgr::GetInst()->Update();
	LevelMgr::GetInst()->Render();

	Device::GetInst()->Present();

	// Task Update
	TaskMgr::GetInst()->Update();

	Vec3 toEye = Vec3(0.f, 0.f, -10.f);
	material mat;
	mat.Ambient = Vec4(0.48f, 0.77f, 0.46f, 1.0f);
	mat.Diffuse = Vec4(0.48f, 0.77f, 0.46f, 1.0f);
	mat.Specular = Vec4(0.2f, 0.2f, 0.2f, 16.0f);

	SpotLight L;

	L.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	L.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	L.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	L.Att = Vec3(1.f, 0.0f, 0.f);
	L.Spot = 1.f;
	L.Range = 10000.0f;
	L.Position = Vec3(0.f, 70.f, 70.f);
	L.Direction = Vec3(0.f, 0.f, -0.5f);

	Vec3 pos = Vec3(0.f, 0.f, 0.f);
	Vec3 Normal = Vec3(0.f, 0.f, 1.f);

	Vec4 ambient = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 diffuse = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 spec = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

	Vec3 lightVec = L.Position - pos;

	float d = lightVec.Length();

	if (d > L.Range)
		return;

	lightVec /= d;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = lightVec.Dot(Normal);

	if (diffuseFactor > 0.0f)
	{
		Vec3 v = -lightVec.Reflect(-lightVec, Normal);
		
		float specFactor = pow(max(v.Dot(toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
	float spot = pow(max(-lightVec.Dot(L.Direction), 0.0f),L.Spot);
	float att = spot / L.Att.Dot(Vec3(1.0f, d, d * d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}
