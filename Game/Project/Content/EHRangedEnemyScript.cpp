#include "pch.h"
#include "EHRangedEnemyScript.h"

#include <EHGameObject.h>
#include <EHAssetMgr.h>
#include "EHProjecTileScript.h"
#include <EHLevelMgr.h>

RangedEnemyScript::RangedEnemyScript()
{
	SetName(L"RangedEnemyScript");
	SetState(EnemyState::Idle);
	SetType(EnemyType::Ranged);
}

RangedEnemyScript::~RangedEnemyScript()
{
}

void RangedEnemyScript::Update()
{
	EnemyScript::Update();
}

void RangedEnemyScript::Idle()
{
}

void RangedEnemyScript::Chase()
{
}

void RangedEnemyScript::Attack()
{
}

void RangedEnemyScript::LaunchMissile(const wstring& _sPlayName, const wstring& _sDeadName, Dir _eDir, float _fSPeed, float _fDamage, Vec4 _vPos, Vec4 _vScale)
{
	GameObject* _pProjecTile = new GameObject();
	MeshRenderer* _pProjecTileRender = _pProjecTile->AddComponent<MeshRenderer>();
	_pProjecTileRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));
	_pProjecTileRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	Transform* _pProjecTileTr = _pProjecTile->AddComponent<Transform>();
	_pProjecTileTr->SetRelativePosition(_vPos);
	_pProjecTileTr->SetRelativeScale(_vScale);
	Collider2D* _pProjecTileCol = _pProjecTile->AddComponent<Collider2D>();
	_pProjecTileCol->SetoffSetScale(Vec3(0.05f, 0.05f, 1.f));
	Animator2D* _pProjecTileAnim = _pProjecTile->AddComponent<Animator2D>();
	_pProjecTileAnim->Play(_sPlayName);
	ProjecTileScript* _pScript = _pProjecTile->AddComponent<ProjecTileScript>();
	_pScript->SetDamage(5);
	_pScript->SetSpeed(500);
	_pScript->SetDir(_eDir);
	_pScript->SetDeadAnimName(_sDeadName);

	LevelMgr::GetInst()->GetCurLevel()->AddObject(_pProjecTile, LAYER_TYPE::ENEMY_PROJECTILE);
}