#include "pch.h"
#include "EHTestLevel.h"

#include "EHPlayer.h"
#include "EHGameObject.h"

#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"

#include "EHCamera.h"

extern Camera* MainCamera;

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Init()
{
	GameObject* _player = new GameObject;
	Transform* tr = _player->AddComponent<Transform>();
	tr->SetScale(Vec4(16.f, 9.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 0.2f, 1.f));
	tr->SetRotation(0.f);

	MeshRenderer* _playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"BackGroundMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"BackGroundShader")));

	AddObject(_player, LAYER_TYPE::BACKGROUND);

	Player* _pla2 = new Player();
	tr = _pla2->AddComponent<Transform>();
	tr->SetScale(Vec4(2.f, 2.f, 1.f, 1.f));
	tr->SetPosition(Vec4(0.f, 0.f, 0.f, 1.f));
	tr->SetRotation(0.f);
	_playerRenderer = _pla2->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(dynamic_cast<Mesh*>(AssetMgr::GetInst()->FindAsset(L"PlayerMesh")));
	_playerRenderer->SetShader(dynamic_cast<GraphicShader*>(AssetMgr::GetInst()->FindAsset(L"PlayerShader")));

	AddObject(_pla2, LAYER_TYPE::PLAYER);
	MainCamera->SetTarget(_pla2);
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