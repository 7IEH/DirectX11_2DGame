#include "pch.h"
#include "EHGolemKingScript.h"

#include <EHGameObject.h>
#include <EHAnimation2D.h>

#include <EHTimeMgr.h>
#include <EHLevelMgr.h>
#include <EHAssetMgr.h>
#include "EHRandomManager.h"

#include "EHGolemKingATK1Script.h"
#include "EHGolemKingATK2Script.h"
#include "EHGolemKingATK3Script.h"
#include "EHGolemKingATK4Script.h"
#include "EHProjecTileScript.h"
#include "EHSpawnRockScript.h"

#include "EHBossUIScript.h"

#include "EHSpawnEffectScript.h"

GolemKingScript::GolemKingScript()
	: m_fAttackTime(5.f)
	, m_fAcctime(0.f)
	, m_bArm(TRUE)
	, m_eState(BossState::Pause)
	, m_ePattern(Pattern::LaunchArm)
	, m_pPlayer(nullptr)
	, m_bTrackCompleted(FALSE)
	, m_bAttackCount(0)
	, m_fWaitingTime1(FALSE)
	, m_pWaveObject(nullptr)
	, m_pArmObject1(nullptr)
	, m_bSpawnRocks(FALSE)
{
	SetName(L"GolemKingScript");
	SetHp(400);
}

GolemKingScript::~GolemKingScript()
{
}

void GolemKingScript::Start()
{
	m_pPlayer = FIND_OBJECT(L"Player");
	m_pWaveObject = FIND_OBJECT(L"Object_Wave");
	m_pArmObject1 = FIND_OBJECT(L"Object_ArmObject1");

	assert(m_pPlayer);
	assert(m_pWaveObject);
	assert(m_pArmObject1);

	m_pWaveObject->Enabled(FALSE);
	m_pWaveObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->SetCurAnimation2D(L"Boss_Golem_Attack_Wave_Anim");
	m_pArmObject1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(0.f, 1300.f, 0.f, -1000.f));

	m_pWaveObject->GetScript<ProjecTileScript>()->SetNotDir(TRUE);
	m_pWaveObject->GetScript<ProjecTileScript>()->SetDamage(15);

	m_pWaveObject->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

	ProjecTileScript* m_pArmScript = m_pArmObject1->GetScript<ProjecTileScript>();
	m_pArmScript->SetDamage(30);
	m_pArmScript->SetNotDir(TRUE);
}

void GolemKingScript::Update()
{
	if (GetHp() == 0)
	{
		m_eState = BossState::Dead;

		m_pWaveObject->Enabled(FALSE);
		m_pArmObject1->Enabled(FALSE);
	}

	switch (m_eState)
	{
	case BossState::Idle:
	{
		Idle();
	}
	break;
	case BossState::Attack:
	{
		Attack();
	}
	break;
	case BossState::Dead:
	{
		Dead();
	}
	break;
	case BossState::Pause:
	{
		Pause();
	}
	break;
	default:
		break;
	}
}

void GolemKingScript::LateUpdtae()
{
}

void GolemKingScript::OnTriggerEnter(Collider* _other)
{
	EnemyScript::OnTriggerEnter(_other);

	GetOwner()->GetScript<BossUIScript>()->SetAttack();
}

void GolemKingScript::Idle()
{
	if (m_pWaveObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		m_pWaveObject->Enabled(FALSE);
		m_pWaveObject->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
	}

	if (m_bArm)
	{
		if (L"Boss_Golem_Idle_Arm_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
		{
			GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_Arm_Anim");
		}
	}

	m_fAcctime += DT;

	if (m_fAcctime >= m_fAttackTime)
	{
		m_ePattern = Pattern(RandomManager::GetInst()->GenerateNumber(0, 1));
		m_eState = BossState::Attack;
		m_fAcctime = 0.f;
	}
}

void GolemKingScript::Attack()
{
	switch (m_ePattern)
	{
	case Pattern::LaunchArm:
	{
		LaunchArm();
	}
	break;
	case Pattern::SpawnRocks:
	{
		SpawnRocks();
	}
	break;
	case Pattern::Sticky:
	{
		Sticky();
	}
	break;
	case Pattern::Wave:
	{
		Wave();
	}
	break;
	default:
		break;
	}
}

void GolemKingScript::Dead()
{
	if (L"Boss_Golem_Dead_Anim2" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		return;
	}

	if (L"Boss_Golem_Dead_Anim2" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& L"Boss_Golem_Dead_Anim1" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Dead_Anim1", FALSE);
		Object::ShakingEffect(4.f, 1000.f, 1.f);
	}

	if (L"Boss_Golem_Dead_Anim1" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Dead_Anim2", FALSE);
	}
}

void GolemKingScript::Pause()
{
	if (m_pWaveObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		m_pWaveObject->Enabled(FALSE);
		m_pWaveObject->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
	}
}

void GolemKingScript::LaunchArm()
{
	if (m_bAttackCount == 5)
	{
		if (L"Boss_Golem_Attack_Launch2_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
			&& m_pArmObject1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y >= 1300.f)
		{
			m_fWaitingTime1 = FALSE;
			GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_Launch2_Anim");
			m_ePattern = Pattern::Wave;
			m_bAttackCount = 0;
		}

		return;
	}

	if (L"Boss_Golem_Attack_Launch1_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		L"Boss_Golem_Idle_NoArm_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() && !m_fWaitingTime1)
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_Launch1_Anim");
	}

	if (L"Boss_Golem_Attack_Launch1_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_NoArm_Anim");
		m_fWaitingTime1 = TRUE;
	}

	if (m_pArmObject1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y >= 1300.f && m_fWaitingTime1)
	{
		m_fAcctime += DT;

		if (m_fAcctime >= 3.f && !m_bTrackCompleted)
		{
			m_fAcctime = 0.f;
			m_bTrackCompleted = TRUE;
			Vec4 _vPos = m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			m_pArmObject1->GetScript<GolemKingATK1Script>()->SetTargetPos(Vec2(_vPos.x, _vPos.y));
			SpawnShadow(Vec2(_vPos.x, _vPos.y));
		}
	}

	if (m_bTrackCompleted)
	{
		m_fAcctime += DT;

		if (m_fAcctime >= 1.f)
		{
			m_bAttackCount += 1;
			m_bTrackCompleted = FALSE;
			m_fAcctime = 0.f;

			m_pArmObject1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_PunchDown_Anim", FALSE);
			m_pArmObject1->GetScript<GolemKingATK1Script>()->SetState(ATK1State::Attack);
		}
	}
}

void GolemKingScript::SpawnRocks()
{
	if (L"Boss_Golem_Attack_RocksSpawn_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_RocksSpawn_Anim", FALSE);
	}

	if (L"Boss_Golem_Attack_RocksSpawn_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		// 로직 시작
		Vec4 _vPlayerPos = m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
		GetOwner()->GetScript<GolemKingATK2Script>()->StartLogic();
		GetOwner()->GetScript<GolemKingATK2Script>()->SetTargetPos(Vec2(_vPlayerPos.x, _vPlayerPos.y));
		m_ePattern = Pattern::Wave;
	}
}

void GolemKingScript::Sticky()
{
	GetOwner()->GetScript<GolemKingATK3Script>()->StartLogic();
	m_eState = BossState::Pause;
}

void GolemKingScript::Wave()
{
	m_pWaveObject->Enabled(TRUE);
	if (L"Boss_Golem_Attack_Launch2_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_Arm_Anim");
	}

	if (L"Boss_Golem_Attack_RocksSpawn_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_Arm_Anim");
		m_bSpawnRocks = TRUE;
	}

	if (L"Boss_Golem_Attack_StickyEnd_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
		&& GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_Arm_Anim");
	}

	m_fAcctime += DT;

	if (m_fAcctime >= 3.f)
	{
		// Animation Object
		m_pWaveObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_Wave_Anim", FALSE);
		m_pWaveObject->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
		m_eState = BossState::Idle;
		m_fAcctime = 0.f;

		if (m_bSpawnRocks)
		{
			m_eState = BossState::Attack;
			m_ePattern = Pattern::Sticky;
			m_bSpawnRocks = FALSE;
		}
	}
}

void GolemKingScript::SpawnShadow(Vec2 _vPos)
{
	Vec4 _v4Pos = Vec4(_vPos.x, _vPos.y, 0.f, 0.f);

	GameObject* _pCrack = new GameObject;
	Transform* _pTr = _pCrack->AddComponent<Transform>();
	MeshRenderer* _pRender = _pCrack->AddComponent<MeshRenderer>();

	_v4Pos.z = 1000.f;
	_pTr->SetRelativePosition(_v4Pos);
	_pTr->SetRelativeScale(Vec4(160.f, 112.5f, 1.f, 1.f));
	_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Boss_Golem_Attack_Punch_Shadow1_Mat"));

	_pCrack->AddComponent<SpawnEffectScript>();

	Object::Instantiate(_pCrack, int(LAYER_TYPE::BACKGROUND2));
}
