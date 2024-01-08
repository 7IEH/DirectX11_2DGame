#include "pch.h"
#include "EHTitleLevel.h"

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

#include "EHTaskMgr.h"

TitleLevel::TitleLevel()
	:m_LeftDoor(nullptr)
	, m_RightDoor(nullptr)
	, m_bOpen(FALSE)
{
}

TitleLevel::~TitleLevel()
{
}


void TitleLevel::Awake()
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

	// Background
	GameObject* _backGround = new GameObject;
	tr = _backGround->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, 60.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _backGround->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));

	Animator2D* _animator = _backGround->AddComponent<Animator2D>();
	Ptr<Sprite> _sprite = AssetMgr::GetInst()->FindAsset<Sprite>(L"_BackGroundAtlas2");
	_animator->CreateAnimation(L"_BackGroundAtlas2", _sprite, Vec2(0.f, 0.f), Vec2(0.f, 0.f), Vec2(640.f, 360.f), Vec2(640.f, 360.f), 16, 10.f);
	_animator->Play(L"_BackGroundAtlas2");



	// Background
	GameObject* _backGround2 = new GameObject;
	tr = _backGround2->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 0.f, 50.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = _backGround2->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));

	_animator = _backGround2->AddComponent<Animator2D>();
	_sprite = AssetMgr::GetInst()->FindAsset<Sprite>(L"_BackGroundAtlas1");
	_animator->CreateAnimation(L"_BackGroundAtlas1", _sprite, Vec2(0.f, 0.f), Vec2(0.f, 0.f), Vec2(640.f, 360.f), Vec2(640.f, 360.f), 16, 10.f);
	_animator->Play(L"_BackGroundAtlas1");



	//// Light
	//GameObject* _Light = new GameObject();
	//tr = _Light->AddComponent<Transform>();
	//LIght2D* _light = _Light->AddComponent<LIght2D>();
	//Object::Instantiate(_Light, (UINT)LAYER_TYPE::LIGHT2D);

	// Right Door
	GameObject* RightDoor = new GameObject;
	tr = RightDoor->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(800.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(-405.f, 0.f, 30.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = RightDoor->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"StartRightDoorMat"));
	m_RightDoor = RightDoor;



	// Left Door
	GameObject* LeftDoor = new GameObject;
	tr = LeftDoor->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(800.f, 900.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(405.f, 0.f, 30.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = LeftDoor->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"StartLeftDoorMat"));
	m_LeftDoor = LeftDoor;

	//// Line
	//GameObject* startLine = new GameObject;
	//tr = startLine->AddComponent<Transform>();
	//tr->SetRelativeScale(Vec4(10.f, 900.f, 1.f, 1.f));
	//tr->SetRelativePosition(Vec4(0.f, 0.f, 30.f, 1.f));
	//tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	//_playerRenderer = startLine->AddComponent<MeshRenderer>();
	//_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	//_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"StartLineMat"));


	// Logo
	GameObject* GameLogo = new GameObject;
	tr = GameLogo->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(352.f, 280.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(0.f, 150.f, 0.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	_playerRenderer = GameLogo->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"GameLogoMat"));


	Object::Instantiate(_backGround, (UINT)LAYER_TYPE::BACKGROUND);
	Object::Instantiate(_backGround2, (UINT)LAYER_TYPE::BACKGROUND);
	Object::Instantiate(RightDoor, (UINT)LAYER_TYPE::BACKGROUND);
	//Object::Instantiate(startLine, (UINT)LAYER_TYPE::BACKGROUND);
	Object::Instantiate(LeftDoor, (UINT)LAYER_TYPE::BACKGROUND);
	Object::Instantiate(GameLogo, (UINT)LAYER_TYPE::BACKGROUND);

	Level::Awake();
}

void TitleLevel::Start()
{
	Level::Start();
}

void TitleLevel::Update()
{
	Level::Update();

	Layer* layer = this->GetLayer(LAYER_TYPE::BACKGROUND);


	if (KEY_TAP(KEY::J))
	{
		m_bOpen = !m_bOpen;
	}

	if (m_bOpen == FALSE)
	{
		Transform* rightTr = m_RightDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Transform* leftTr = m_LeftDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		Vec4 _rightPos = rightTr->GetRelativePosition();
		Vec4 _leftPos = leftTr->GetRelativePosition();

		_leftPos.x += (-410.f - _leftPos.x) * DT * 10.f;
		_rightPos.x += (410.f - _rightPos.x) * DT * 10.f;

		rightTr->SetRelativePosition(_rightPos);
		leftTr->SetRelativePosition(_leftPos);
	}
	else
	{
		Transform* rightTr = m_RightDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Transform* leftTr = m_LeftDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		Vec4 _rightPos = rightTr->GetRelativePosition();
		Vec4 _leftPos = leftTr->GetRelativePosition();

		_leftPos.x += (-1100.f - _leftPos.x) * DT * 10.f;
		_rightPos.x += (1100.f - _rightPos.x) * DT * 10.f;

		rightTr->SetRelativePosition(_rightPos);
		leftTr->SetRelativePosition(_leftPos);
	}
}

void TitleLevel::FixedUpdate()
{
	Level::FixedUpdate();
}

void TitleLevel::LateUpdate()
{
	Level::LateUpdate();
}