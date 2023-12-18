#include "pch.h"
#include "EHTestLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHCamera.h"

#include "EHPlayerScript.h"
#include "EHCameraScript.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Init()
{
	GameObject* _MainCamera = new GameObject;
	Transform* tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraScript* _cameraScript = _MainCamera->AddComponent<CameraScript>();
	tr->SetScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, -20.f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	AddObject(_MainCamera, LAYER_TYPE::CAMERA);

	GameObject* _backGround = new GameObject;
	tr = _backGround->AddComponent<Transform>();
	tr->SetScale(Vec4(16.f, 9.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 0.2f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _backGround->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"BackGroundMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"BackGroundShader")));

	AddObject(_backGround, LAYER_TYPE::BACKGROUND);

	GameObject* _player = new GameObject();
	tr = _player->AddComponent<Transform>();
	tr->SetScale(Vec4(2.f, 2.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 0.f, 1.f));
	tr->SetRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"PlayerMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"PlayerShader")));

	PlayerScript* _playerScript = _player->AddComponent<PlayerScript>();

	AddObject(_player, LAYER_TYPE::PLAYER);

	_cameraScript->SetTarget(_player);
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