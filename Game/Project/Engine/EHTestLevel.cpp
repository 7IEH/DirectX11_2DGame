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
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND, TRUE);
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

	//// Light
	//GameObject* _Light = new GameObject();
	//tr = _Light->AddComponent<Transform>();
	//LIght2D* _light = _Light->AddComponent<LIght2D>();


	//Object::Instantiate(_Light, (UINT)LAYER_TYPE::LIGHT2D);
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

	Animator2D* _animator = _player->AddComponent<Animator2D>();
	Ptr<Sprite> _sprite = AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerIdleSprite");
	_animator->CreateAnimation(L"PlayerIdle", _sprite, Vec2(0.f, 0.f), Vec2(0.f, 0.f), Vec2(23.f, 25.f), Vec2(200.f, 200.f), 5, 10.f);
	_animator->Play(L"PlayerIdle");

	ImGUIMgr::GetInst()->SetObject(_player);

	CircleCollider2D* _col = _player->AddComponent<CircleCollider2D>();
	_col->SetRadius(100.f);

	PlayerScript* _playerScript = _player->AddComponent<PlayerScript>();

	Object::Instantiate(_player, (UINT)LAYER_TYPE::PLAYER);
#pragma endregion

#pragma region Script Option
	_cameraScript->SetTarget(_player);
#pragma endregion


	// Player
	GameObject* _enemy = new GameObject();
	tr = _enemy->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(100.f, 92.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(450.f, 0.f, 0.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = _enemy->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"PlayerMaterial"));

	_col = _enemy->AddComponent<CircleCollider2D>();
	_col->SetRadius(100.f);

	Object::Instantiate(_enemy, (UINT)LAYER_TYPE::MONSTER);

	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::MONSTER);

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