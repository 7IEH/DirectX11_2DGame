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
#include "EHPathMgr.h"

#include "EHTestComputeShader.h"

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
	GameObject* _test = nullptr;

	GameObject* _MainCamera = new GameObject;
	Transform* tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraScript* _cameraScript = _MainCamera->AddComponent<CameraScript>();

	_camera->AllVisibleSet(TRUE);
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND1, TRUE);
	_camera->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);

	tr->SetRelativeScale(Vec4(1.f, 1.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, -10.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	AddObject(_MainCamera, LAYER_TYPE::CAMERA);
#pragma endregion
	GameObject* _light = new GameObject;
	_light->SetName(L"Light");
	_light->AddComponent<Transform>();

	_light->AddComponent<LIght2D>();

	AddObject(_light, LAYER_TYPE::LIGHT2D);
#pragma endregion

	//GameObject* _tile = new GameObject;
	//_tile->SetName(L"TileTest");
	//tr = _tile->AddComponent<Transform>();
	//tr->SetRelativePosition(Vec4(0.f, 0.f, 30.f, 1.f));
	//TileMap* _tileMap = _tile->AddComponent<TileMap>();

	//_tileMap->SetTileAtlas(AssetMgr::GetInst()->FindAsset<Sprite>(L"TileSprite"), Vec2(64.f, 64.f));

	//for (int y = 0;y < 2;y++)
	//{
	//	for (int x = 0;x < 2;x++)
	//	{
	//		_tileMap->SetTileIndex(y, x, y * 2 + x, 1);
	//	}
	//}

	//Object::Instantiate(_tile, int(LAYER_TYPE::TILE));

	GameObject* _paritcle = new GameObject;
	_paritcle->SetName(L"Particle");
	tr = _paritcle->AddComponent<Transform>();
	ParticleSystem* _particleSys = _paritcle->AddComponent<ParticleSystem>();
	_particleSys->SetParticleSprite(AssetMgr::GetInst()->FindAsset<Sprite>(L"TitleParticle"));
	_particleSys->GetMaterial()->SetMaterialParam(INT_0, 1);
	_particleSys->SetNoiseSprite(AssetMgr::GetInst()->FindAsset<Sprite>(L"Noise5"));

	tParticleModule _module = {};
	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
	_module._SpawnShape = 2;
	_module._SpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	_module._SpawnMinScale = Vec4(12.f, 12.f, 1.f, 1.f);
	_module._SpawnMaxScale = Vec4(12.f, 12.f, 1.f, 1.f);
	_module._SpaceType = 1;
	_module._SpawnRate = 100;
	_module._MinLife = 5.f;
	_module._MaxLife = 20.f;
	_module._iLoop = 1;

	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::COLOR_OVER_LIFETIME] = 1;
	_module._ColorType = 0;

	_particleSys->SetModule(_module);

	tParticle _arrParticle[14] = {};
	_arrParticle[0]._LocalPos = Vec4(100.f, 100.f, 1.f, 0.f);
	_arrParticle[0]._iFadeVariable = 1;
	_arrParticle[1]._LocalPos = Vec4(-100.f, 200.f, 1.f, 0.f);
	_arrParticle[2]._LocalPos = Vec4(500.f, -10.f, 1.f, 0.f);
	_arrParticle[2]._iFadeVariable = 1;
	_arrParticle[3]._LocalPos = Vec4(-200.f, -200.f, 1.f, 0.f);
	_arrParticle[4]._LocalPos = Vec4(300.f, -50.f, 1.f, 0.f);
	_arrParticle[4]._iFadeVariable = 1;
	_arrParticle[5]._LocalPos = Vec4(-100.f, -200.f, 1.f, 0.f);
	_arrParticle[6]._LocalPos = Vec4(-200.f, 100.f, 1.f, 0.f);
	_arrParticle[6]._iFadeVariable = 1;

	_arrParticle[7]._LocalPos = Vec4(500.f, 100.f, 1.f, 0.f);
	_arrParticle[8]._LocalPos = Vec4(-400.f, 200.f, 1.f, 0.f);
	_arrParticle[8]._iFadeVariable = 1;
	_arrParticle[9]._LocalPos = Vec4(300.f, -100.f, 1.f, 0.f);
	_arrParticle[10]._LocalPos = Vec4(-200.f, -200.f, 1.f, 0.f);
	_arrParticle[11]._LocalPos = Vec4(300.f, -500.f, 1.f, 0.f);
	_arrParticle[12]._LocalPos = Vec4(-100.f, -500.f, 1.f, 0.f);
	_arrParticle[12]._iFadeVariable = 1;
	_arrParticle[13]._LocalPos = Vec4(-200.f, 300.f, 1.f, 0.f);

	_particleSys->GetParticleBuffer()->SetData((void*)&_arrParticle, 14);

	tr->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 0.f));

	AddObject(_paritcle, LAYER_TYPE::PARTICLE);

	// Player
	GameObject* _player = new GameObject();
	_player->SetName(L"Player");
	tr = _player->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(96.f, 164.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(400.f, 0.f, -5.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"PlayerMaterial"));

	Collider2D* _playercol = _player->AddComponent<Collider2D>();

	_player->AddComponent<PlayerScript>();

	Animator2D* _animator = _player->AddComponent<Animator2D>();
	_animator->Play(L"IdleFront");

	AddObject(_player, LAYER_TYPE::PLAYER);


	GameObject* _testObject = new GameObject();
	tr = _testObject->AddComponent<Transform>();

	_testObject->AddComponent<CanvasRenderer>();

	Text* _text = _testObject->AddComponent<Text>();
	_text->SetText(L"»õ °ÔÀÓ");
	_text->SetFont(L"DroidSansFallback");
	_text->SetFontWeight(DWRITE_FONT_WEIGHT_SEMI_BOLD);
	_text->SetFontSize(24.f);

	AddObject(_testObject, LAYER_TYPE::PARTICLE);

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