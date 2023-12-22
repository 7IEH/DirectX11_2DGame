#include "pch.h"
#include "EHTestLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"

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

void TestLevel::Init()
{
	#pragma region Essential Object
	// Camera
	GameObject* _MainCamera = new GameObject;
	Transform* tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraTargetScript* _cameraScript = _MainCamera->AddComponent<CameraTargetScript>();
	tr->SetScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, -10.f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	// Light
	GameObject* _Light = new GameObject();
	tr = _Light->AddComponent<Transform>();
	LIght2D* _light = _Light->AddComponent<LIght2D>();

	AddObject(_MainCamera, LAYER_TYPE::CAMERA);
	AddObject(_Light, LAYER_TYPE::LIGHT2D);
	#pragma endregion

	#pragma region BackGround
	// Background
	GameObject* _backGround = new GameObject;
	tr = _backGround->AddComponent<Transform>();
	tr->SetScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 50.f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _backGround->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"BackGroundMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"BackGroundShader")));
	_playerRenderer->SetSprite(dynamic_cast<Sprite*>(AssetMgr::GetInst()->FindAsset(L"BackGroundSprite")));
	AddObject(_backGround, LAYER_TYPE::BACKGROUND);

	#pragma endregion

	#pragma region GameObject
	// Player
	GameObject* _player = new GameObject();
	tr = _player->AddComponent<Transform>();
	tr->SetScale(Vec4(100.f, 92.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 0.f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"PlayerMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"PlayerShader")));
	_playerRenderer->SetMaterial(dynamic_cast<Material*>(AssetMgr::GetInst()->FindAsset(L"DefaultMat")));
	_playerRenderer->SetSprite(dynamic_cast<Sprite*>(AssetMgr::GetInst()->FindAsset(L"PlayerSprite")));

	PlayerScript* _playerScript = _player->AddComponent<PlayerScript>();

	AddObject(_player, LAYER_TYPE::PLAYER);
	#pragma endregion

	#pragma region Script Option
	_cameraScript->SetTarget(_player);
	#pragma endregion

	Level::Init();
}

void TestLevel::Tick()
{
	Level::Tick();
}

void TestLevel::FinalTick()
{
	Level::FinalTick();
}

void TestLevel::Render()
{
	Level::Render();
}