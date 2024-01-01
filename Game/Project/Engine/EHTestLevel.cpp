#include "pch.h"
#include "EHTestLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"
#include "EHThreadMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h" 

#include "EHPlayerScript.h"
#include "EHCameraScript.h"
#include "EHCameraTargetScript.h"

#include "EHCamera.h"
#include "EHLIght2D.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Awake()
{
	#pragma region Essential Object
	// Main Camera
	GameObject* _MainCamera = new GameObject;
	Transform* tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraScript* _cameraScript = _MainCamera->AddComponent<CameraScript>();
	
	_camera->AllVisibleSet(TRUE);
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND,true);
	_camera->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);

	tr->SetRelativeScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, -10.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	// UI Camera
	/*GameObject* _UICamera = new GameObject;
	tr = _UICamera->AddComponent<Transform>();
	_camera = _UICamera->AddComponent<Camera>();

	_camera->AllVisibleSet(TRUE);
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND, TRUE);
	_camera->SetCameraType(CAMERA_TYPE::UI_CAMERA);

	tr->SetRelativeScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, -10.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	
	Object::Instantiate(_UICamera, (UINT)LAYER_TYPE::CAMERA);*/

	// Light
	GameObject* _Light = new GameObject();
	tr = _Light->AddComponent<Transform>();
	LIght2D* _light = _Light->AddComponent<LIght2D>();

	
	Object::Instantiate(_Light, (UINT)LAYER_TYPE::LIGHT2D);
	Object::Instantiate(_MainCamera, (UINT)LAYER_TYPE::CAMERA);
	#pragma endregion

	#pragma region BackGround
	// Background
	GameObject* _backGround = new GameObject;
	tr = _backGround->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, 50.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _backGround->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));
	Object::Instantiate(_backGround, (UINT)LAYER_TYPE::BACKGROUND);
	#pragma endregion

	#pragma region GameObject
	// Player
	GameObject* _player = new GameObject();
	tr = _player->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(100.f, 92.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"PlayerMaterial"));

	PlayerScript* _playerScript = _player->AddComponent<PlayerScript>();

	Object::Instantiate(_player, (UINT)LAYER_TYPE::PLAYER);
	#pragma endregion

	#pragma region Script Option
	_cameraScript->SetTarget(_player);
	#pragma endregion
	
	//Object::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(400.f, 400.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 0.f, 0.f), TRUE, 20.f);
	Level::Awake();
}

void TestLevel::Start()
{
	Level::Start();
}

void TestLevel::Update()
{
	Level::Update();
}

void TestLevel::FixedUpdate()
{
	Level::FixedUpdate();
}

void TestLevel::LateUpdate()
{
	Level::LateUpdate();
}