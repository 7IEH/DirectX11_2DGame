#include "pch.h"
#include "EHIntroLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"
#include "EHThreadMgr.h"
#include "EHImGUIMgr.h"

#include "EHMesh.h"
#include "EHSprite.h"
#include "EHMaterial.h" 

#include "EHBehaviour.h"
#include "EHCollisionMgr.h"
#include "EHTimeMgr.h"
#include "EHKeyMgr.h"
#include "EHLevelMgr.h"
#include "EHPathMgr.h"

#include "EHDevice.h"
#include "EHRenderMgr.h"

// 동영상 재생
#include <Vfw.h>
#pragma comment(lib,"vfw32.lib")

IntroLevel::IntroLevel()
	:m_PlayTime(0.f)
	, m_video(nullptr)
{
}

IntroLevel::~IntroLevel()
{
}

void IntroLevel::Awake()
{
	// Main Camera
	GameObject* _MainCamera = new GameObject;
	Transform* tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraScript* _cameraScript = _MainCamera->AddComponent<CameraScript>();

	_camera->AllVisibleSet(TRUE);
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND, TRUE);
	_camera->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);

	tr->SetRelativeScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, -10.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	Object::Instantiate(_MainCamera, (UINT)LAYER_TYPE::CAMERA);

	wstring _finalpath = PATH;
	wstring _relativePath = L"\\resource\\Intro\\Video\\Intro.mp4";
	_finalpath += _relativePath;
	m_video = MCIWndCreate(MAIN_HWND, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD, _finalpath.c_str());
	MoveWindow(m_video, 0, 0, 1600, 900, NULL);
	MCIWndPlay(m_video);
	RenderMgr::GetInst()->SetRender(FALSE);
}

void IntroLevel::Start()
{
	
}

void IntroLevel::Update()
{
	m_PlayTime += DT;

	if (18.f <= m_PlayTime)
	{
		MCIWndStop(m_video);
		MCIWndClose(m_video);
		MCIWndDestroy(m_video);
		SceneManager::LoadScene(L"TitleLevel");
		RenderMgr::GetInst()->SetRender(TRUE);
		m_PlayTime = 0.f;
	}
}

void IntroLevel::FixedUpdate()
{

}

void IntroLevel::LateUpdate()
{
}