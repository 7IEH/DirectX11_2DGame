#include "pch.h"
#include "EHEnemyScript.h"

#include <EHGameObject.h>
#include <EHLevelMgr.h>
#include "EHRecordManager.h"

#include <EHAssetMgr.h>
#include <EHTimeMgr.h>

EnemyScript::EnemyScript()
	: m_eState(EnemyState::Idle)
	, m_eEnemyType(EnemyType::Melee)
	, m_iHp(0)
	, m_eChase(FALSE)
	, m_eDir(EnemyDir::UP)
	, m_fAccTime(0.f)
	, m_fChargeTime(0.f)
	, m_iDamage(0)
{
	SetName(L"EnemyScript");
}

EnemyScript::~EnemyScript()
{
}

void EnemyScript::Awake()
{
}

void EnemyScript::Start()
{
	GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()
		->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"EnemyShader"));
}

void EnemyScript::Update()
{
	if (m_iHp <= 0)
	{
		m_eState = EnemyState::Dead;
	}

	if (m_bDamaged)
	{
		m_fAccTime2 += DT;
		if (m_fAccTime2 >= 0.5f)
		{
			m_bDamaged = FALSE;
			GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_0, 0);
			m_fAccTime2 = 0.f;
		}
	}

	switch (m_eState)
	{
	case EnemyState::Idle:
	{
		Idle();
	}
	break;
	case EnemyState::Attack:
	{
		Attack();
	}
	break;
	case EnemyState::Chase:
	{
		Chase();
	}
	break;
	case EnemyState::Dead:
	{
		Dead();
	}
	case EnemyState::None:
		break;
	default:
		break;
	}
}

void EnemyScript::LateUpdate()
{
}

void EnemyScript::OnTriggerEnter(Collider* _other)
{
	LAYER_TYPE _eLayer = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::PLAYER_PROJECTILE == _eLayer)
	{
		GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_0, 1);
		m_bDamaged = TRUE;

		// Monster Hp
		int _fStrikingPower = RecordManager::GetInst()->GetCurrentPlayerPref()->_iStrikingPower;

		Dir _ePlayerDir = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<PlayerScript>()->GetDirection();
		Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vPos = _pTr->GetRelativePosition();

		m_iHp -= _fStrikingPower;

		switch (_ePlayerDir)
		{
		case Dir::UP:
		{
			_vPos.y += 2.f;
		}
		break;
		case Dir::DOWN:
		{
			_vPos.y -= 2.f;
		}
		break;
		case Dir::LEFT:
		{
			_vPos.x -= 2.f;
		}
		break;
		case Dir::RIGHT:
		{
			_vPos.x += 2.f;
		}
		break;
		default:
			break;
		}

		if (m_iHp <= 0)
			m_iHp = 0;
		_pTr->SetRelativePosition(_vPos);
	}
}

void EnemyScript::Dead()
{
	// FadeOut Ãß°¡
	//GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled();
	GetOwner()->SetDead(TRUE);
}

void EnemyScript::CaculateDir()
{
	Transform* _pPlayerTr = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")
		->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	assert(_pPlayerTr);

	Transform* _pTangleTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _vPlayerPos = _pPlayerTr->GetRelativePosition();
	Vec4 _vTanglePos = _pTangleTr->GetRelativePosition();

	Vec4 _vDifference = _vPlayerPos - _vTanglePos;

	float _fRadian = 0.f;
	float _fDegree = 0.f;
	if (_vDifference.x >= 0.f && _vDifference.y >= 0.f)
	{
		_fRadian = atanf(_vDifference.y / _vDifference.x);

		_fDegree = _fRadian * (180.f / 3.14f);
	}
	else if (_vDifference.x <= 0.f && _vDifference.y >= 0.f)
	{
		_fRadian = atanf(_vDifference.y / _vDifference.x);

		_fDegree = _fRadian * (180.f / 3.14f) + 180.f;
	}
	else if (_vDifference.x <= 0.f && _vDifference.y <= 0.f)
	{
		_fRadian = atanf(_vDifference.y / _vDifference.x);

		_fDegree = _fRadian * (180.f / 3.14f) + 180.f;
	}
	else
	{
		_fRadian = atanf(_vDifference.y / _vDifference.x);

		_fDegree = _fRadian * (180.f / 3.14f) + 360.f;
	}


	if (_fDegree <= 45.f || _fDegree >= 315.f)
	{
		SetDir(EnemyDir::RIGHT);
	}
	else if (_fDegree >= 45.f && _fDegree <= 135.f)
	{
		SetDir(EnemyDir::UP);
	}
	else if (_fDegree >= 135.f && _fDegree <= 225.f)
	{
		SetDir(EnemyDir::LEFT);
	}
	else
	{
		SetDir(EnemyDir::DOWN);
	}
}