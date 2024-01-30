#include "pch.h"
#include "EHTestLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"
#include "EHThreadMgr.h"
#include "EHImGUIMgr.h"

#include "EHMesh.h"
#include "EHSprite.h"
#include "EHMaterial.h" 

#include "EHBehaviour.h"
#include "EHCollisionMgr.h"

#include "EHTestComputeShader.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Awake()
{
	// ComputeShader 테스트
	// 사용할 텍스쳐 생성
	Ptr<Sprite> pTestTex = AssetMgr::GetInst()->CreateResoruceTexture(L"TestTex"
		, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	Ptr<TestComputeShader> pCS = (TestComputeShader*)AssetMgr::GetInst()->FindAsset<ComputeShader>(L"SetColorShader").Get();
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

#pragma region Essential Object
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
#pragma endregion
	GameObject* _light = new GameObject;
	_light->SetName(L"Light");
	_light->AddComponent<Transform>();

	_light->AddComponent<LIght2D>();

	Object::Instantiate(_light, (UINT)LAYER_TYPE::LIGHT2D);
#pragma region BackGround
	// Background
	GameObject* _backGround = new GameObject;
	tr = _backGround->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, 100.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _backGround->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));
	_playerRenderer->GetMaterial()->SetTexParam(TEX_0, pTestTex);
	Object::Instantiate(_backGround, (UINT)LAYER_TYPE::BACKGROUND);
#pragma endregion

	GameObject* _tile = new GameObject;
	_tile->SetName(L"TileTest");
	tr = _tile->AddComponent<Transform>();
	tr->SetRelativePosition(Vec4(0.f, 0.f, 30.f, 1.f));
	TileMap* _tileMap = _tile->AddComponent<TileMap>();

	_tileMap->SetTileAtlas(AssetMgr::GetInst()->FindAsset<Sprite>(L"TileSprite"), Vec2(64.f, 64.f));

	for (int y = 0;y < 2;y++)
	{
		for (int x = 0;x < 2;x++)
		{
			_tileMap->SetTileIndex(y, x, y * 2 + x, 1);
		}
	}

	Object::Instantiate(_tile, int(LAYER_TYPE::TILE));


	GameObject* _paritcle = new GameObject;
	_paritcle->SetName(L"Particle");
	tr = _paritcle->AddComponent<Transform>();
	ParticleSystem* _particleSys = _paritcle->AddComponent<ParticleSystem>();

	tr->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 0.f));

	Object::Instantiate(_paritcle, int(LAYER_TYPE::PLAYER));
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