#include "pch.h"
#include "EHGolemKingATK2Script.h"

#include "EHRandomManager.h"

#include <EHGameObject.h>

#include <EHAssetMgr.h>
#include <EHLevelMgr.h>
#include "EHSpawnRockScript.h"

#include "EHSpawnEffectScript.h"

GolemKingATK2Script::GolemKingATK2Script()
	:m_eShape(SpawnRockShape::Cake)
	, m_eState(SpawnRockState::Idle)
	, m_vTargetPos{}
{
	SetName(L"GolemKingATK2Script");
}

GolemKingATK2Script::~GolemKingATK2Script()
{
}

void GolemKingATK2Script::Start()
{
}

void GolemKingATK2Script::Update()
{
	switch (m_eState)
	{
	case SpawnRockState::Prepare:
	{
		Prepare();
	}
	break;
	case SpawnRockState::Spawn:
	{
		Spawn();
	}
	break;
	default:
		break;
	}
}

void GolemKingATK2Script::LateUpdate()
{

}

void GolemKingATK2Script::Prepare()
{
	m_eShape = SpawnRockShape(RandomManager::GetInst()->GenerateNumber(0, 1));
	m_eState = SpawnRockState::Spawn;
}

void GolemKingATK2Script::Spawn()
{
	vector<GameObject*> _pSpawnRocks; _pSpawnRocks.clear();

	float _fRadius = 0.f;

	switch (m_eShape)
	{
	case SpawnRockShape::Floor:
	{
		SpawnIndividualFloor(400.f, 10, _pSpawnRocks);
		SpawnIndividualFloor(650.f, 20, _pSpawnRocks);
		SpawnIndividualFloor(950.f, 30, _pSpawnRocks);
	}
	break;
	case SpawnRockShape::Cake:
	{
		GameObject* _pGolemBoss = FIND_OBJECT(L"GolemBoss");
		Vec4 _vPos = _pGolemBoss->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
		float _fRadian = atan2f(m_vTargetPos.y - _vPos.y, m_vTargetPos.x - _vPos.x);

		SpawnIndividualCake(_fRadian, _pSpawnRocks);
	}
	break;
	case SpawnRockShape::None:
		break;
	default:
		break;
	}

	for (size_t i = 0;i < _pSpawnRocks.size();i++)
	{
		Object::Instantiate(_pSpawnRocks[i], int(LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK));
	}

	_pSpawnRocks.clear();
	m_eState = SpawnRockState::Idle;
}

void GolemKingATK2Script::SpawnIndividualFloor(float _fRadius, int _idx, OUT vector<GameObject*>& _vObj)
{
	float _fScale[6][2] =
	{
		{97.5f,105.f},
		{95.f,107.5f},
		{100.f,107.5f},
		{97.5f,105.f},
		{92.5f,100.f},
		{100.f,107.5f}
	};

	int _iRand1 = RandomManager::GetInst()->GenerateNumber(0, _idx / 2 - 1);
	int _iRand2 = RandomManager::GetInst()->GenerateNumber(_idx / 2, _idx);

	for (int i = 0;i < _idx;i++)
	{
		if (_iRand1 == i)
			continue;

		if (_iRand2 == i)
			continue;

		GameObject* _rock = new GameObject;
		Transform* _pTr = _rock->AddComponent<Transform>();
		MeshRenderer* _pRender = _rock->AddComponent<MeshRenderer>();
		Collider2D* _pCol = _rock->AddComponent<Collider2D>();
		SpawnRockScript* _pScript = _rock->AddComponent<SpawnRockScript>();

		_pCol->Enabled(FALSE);

		float _fRadian = ((i * (180.f / _idx)) + 180.f) * (3.14f / 180.f);

		float _fCurX = cosf(_fRadian) * _fRadius;
		float _fCurY = 289.f + sinf(_fRadian) * _fRadius;
		int _idx = RandomManager::GetInst()->GenerateNumber(1, 6);

		_pScript->SetTarget(_fCurY);

		_pTr->SetRelativePosition(Vec4(_fCurX, 1300.f, _fCurY, 0.f));
		_pTr->SetRelativeScale(Vec4(_fScale[_idx][0], _fScale[_idx][1], 1.f, 1.f));

		_pCol->SetoffSetScale(Vec3(0.5f, 0.5f, 1.f));

		_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
		_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Object_Boss_Rock_Mat" + std::to_wstring(_idx)));
		_vObj.push_back(_rock);

		SpawnShadow(Vec4(_fCurX, _fCurY, 1.f, 0.f));
	}
}

void GolemKingATK2Script::SpawnIndividualCake(float _fRadian, OUT vector<GameObject*>& _vObj)
{
	float _fScale[6][2] =
	{
		{97.5f,105.f},
		{95.f,107.5f},
		{100.f,107.5f},
		{97.5f,105.f},
		{92.5f,100.f},
		{100.f,107.5f}
	};

	_fRadian -= (15.f) * (3.14f / 180.f);

	for (int i = 0;i < 10;i++)
	{
		for (int j = 0;j < i + 1;j++)
		{
			GameObject* _pRock = new GameObject;
			Transform* _pTr = _pRock->AddComponent<Transform>();
			MeshRenderer* _pRender = _pRock->AddComponent<MeshRenderer>();
			Collider2D* _pCol = _pRock->AddComponent<Collider2D>();
			SpawnRockScript* _pScript = _pRock->AddComponent<SpawnRockScript>();

			float _fRadian2 = _fRadian + (30.f / (i + 1)) * j * (3.14f / 180.f);

			_pCol->Enabled(FALSE);

			float _fCurX = 51.f + cosf(_fRadian2) * 100.f * (i + 1);
			float _fCurY = 289.f + sinf(_fRadian2) * 100.f * (i + 1);
			int _idx = RandomManager::GetInst()->GenerateNumber(1, 6);

			_pScript->SetTarget(_fCurY);

			_pTr->SetRelativePosition(Vec4(_fCurX, 1300.f, _fCurY, 0.f));
			_pTr->SetRelativeScale(Vec4(_fScale[_idx][0], _fScale[_idx][1], 1.f, 1.f));

			_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
			_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Object_Boss_Rock_Mat" + std::to_wstring(_idx)));
			_vObj.push_back(_pRock);
		}
	}
}

void GolemKingATK2Script::SpawnShadow(Vec4 _vPosition)
{
	GameObject* _pCrack = new GameObject;
	Transform* _pTr = _pCrack->AddComponent<Transform>();
	MeshRenderer* _pRender = _pCrack->AddComponent<MeshRenderer>();

	_vPosition.z = 1000.f;
	_pTr->SetRelativePosition(_vPosition);
	_pTr->SetRelativeScale(Vec4(107.5f, 85.f, 1.f, 1.f));
	_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Boss_Golem_Attack_Punch_Shadow1_Mat"));

	SpawnEffectScript* _pScript = _pCrack->AddComponent<SpawnEffectScript>();
	_pScript->SetDeleteTime(1.5f);

	Object::Instantiate(_pCrack, int(LAYER_TYPE::BACKGROUND2));
}