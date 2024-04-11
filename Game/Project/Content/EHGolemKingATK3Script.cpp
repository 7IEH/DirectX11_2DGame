#include "pch.h"
#include "EHGolemKingATK3Script.h"

#include <EHLevelMgr.h>
#include <EHAnimation2D.h>
#include <EHTimeMgr.h>

#include "EHGolemKingScript.h"
#include "EHProjecTileScript.h"

GolemKingATK3Script::GolemKingATK3Script()
	:m_eState(StickyState::Idle)
	, m_pPlayer(nullptr)
	, m_pFist(nullptr)
	, m_pFistModule(nullptr)
	, m_pFistModule2(nullptr)
	, m_pCam(nullptr)
	, m_fAimTime(0.f)
	, m_fLaunchRadian(0.f)
	, m_fLaunchRadius(120.f)
	, m_bCycle(TRUE)
	, m_eSwing(SwingDir::Left)
	, m_fSpeed(1.5f)
{
	SetName(L"GolemKingATK3Script");
}

GolemKingATK3Script::~GolemKingATK3Script()
{
}

void GolemKingATK3Script::Start()
{
	m_pPlayer = FIND_OBJECT(L"Player");
	m_pFist = FIND_OBJECT(L"Object_Golem_King_Fist");
	m_pFistModule = FIND_OBJECT(L"Object_Golem_King_Fist_Module");
	m_pFistModule2 = FIND_OBJECT(L"Object_Golem_King_Fist_Module2");
	m_pCam = FIND_OBJECT(L"MainCamera");

	assert(m_pPlayer);
	assert(m_pCam);
	assert(m_pFist);
	assert(m_pFistModule);
	assert(m_pFistModule2);

	m_pFist->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
	m_pFistModule->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
	m_pFistModule2->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

	m_pFist->Enabled(FALSE);
	m_pFistModule->Enabled(FALSE);
	m_pFistModule2->Enabled(FALSE);

	m_pFist->GetScript<ProjecTileScript>()->SetNotDir(TRUE);
	m_pFistModule->GetScript<ProjecTileScript>()->SetNotDir(TRUE);
	m_pFistModule2->GetScript<ProjecTileScript>()->SetNotDir(TRUE);

	m_pFist->GetScript<ProjecTileScript>()->SetDamage(40);
	m_pFistModule->GetScript<ProjecTileScript>()->SetDamage(40);
	m_pFistModule2->GetScript<ProjecTileScript>()->SetDamage(40);
}

void GolemKingATK3Script::Update()
{
	switch (m_eState)
	{
	case StickyState::Prepare:
	{
		Prepare();
	}
	break;
	case StickyState::Cycle1:
	{
		Cycle1();
	}
	break;
	case StickyState::Cycle2:
	{
		Cycle2();
	}
	break;
	case StickyState::Launch:
	{
		Launch();
	}
	break;
	case StickyState::End:
	{
		End();
	}
	break;
	default:
		break;
	}
}

void GolemKingATK3Script::Prepare()
{
	if (L"Boss_Golem_Attack_StickyCycle_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
	{
		m_fAimTime += DT;
		if (m_fAimTime >= 3.f)
		{
			m_eState = StickyState::Launch;
			m_fAimTime = 0.f;
			return;
		}
		Vec4 _vPlayerPos = m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
		Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

		Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistPos = _pFistTr->GetRelativePosition();

		Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

		// 주먹 원 센터 -> -89 460 module 반지름80 fist 반지름110
		float _fRadian = atan2f(_vPlayerPos.y - 460.f, _vPlayerPos.x + 89.f);
		float _fDegree = XMConvertToDegrees(_fRadian);

		m_fLaunchRadian = _fRadian;

		float _dirX = -89.f + cosf(_fRadian) * 180.f;
		float _dirY = 460.f + sinf(_fRadian) * 180.f;

		_pFistTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree));
		_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));

		// 주먹 원 Module
		_dirX = -89.f + cosf(_fRadian) * 120.f;
		_dirY = 460.f + sinf(_fRadian) * 120.f;

		_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));
		_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));

		Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		_dirX = _vFistModulePos.x + cosf(_fRadian) * 60.f;
		_dirY = _vFistModulePos.y + sinf(_fRadian) * 60.f;

		_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr2->SetRelativeScale(Vec4(50.f, m_fLaunchRadius, _fDegree, 1.f));
		_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));
		return;
	}

	if (L"Boss_Golem_Attack_StickyPrePare_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_StickyPrePare_Anim", FALSE);
	}

	if (L"Boss_Golem_Attack_StickyPrePare_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_StickyCycle_Anim", TRUE);
		m_pFist->Enabled(TRUE);
		m_pFistModule->Enabled(TRUE);
		m_pFistModule2->Enabled(TRUE);

		m_pFist->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
		m_pFistModule->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
		m_pFistModule2->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
	}
}

void GolemKingATK3Script::Cycle1()
{
	// 0 ~ -180.f
	if (m_bCycle)
	{
		float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

		if (_fDegree >= -90.f)
		{
			m_eSwing = SwingDir::Right;
		}
		else
		{
			m_eSwing = SwingDir::Left;
		}

		m_bCycle = FALSE;
	}
	else
	{
		m_fAimTime += DT;
		if (m_fAimTime <= 1.f)
		{
			return;
		}

		if (SwingDir::Left == m_eSwing)
		{
			m_fLaunchRadian += DT * m_fSpeed;
			m_fSpeed += DT * 0.03f;

			float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

			float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
			float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

			// Fist
			Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vFistPos = _pFistTr->GetRelativePosition();

			_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree));

			// FistModule1
			Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

			_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
			_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

			_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

			// FistModule2
			Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

			_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
			_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

			_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

			if (_fDegree >= 0.f)
			{
				Object::ShakingEffect(1.5f, 2000.f, 1.f);
				m_fSpeed = 1.5f;
				m_eState = StickyState::Cycle2;
				m_fAimTime = 0.f;
			}
		}
		else
		{
			m_fAimTime += DT;
			if (m_fAimTime <= 1.f)
			{
				return;
			}

			m_fLaunchRadian -= DT * m_fSpeed;
			m_fSpeed += DT * 0.03f;

			float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

			float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
			float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

			// Fist
			Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vFistPos = _pFistTr->GetRelativePosition();

			_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree));

			// FistModule1
			Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

			_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
			_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

			_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

			// FistModule2
			Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

			_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
			_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

			_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
			_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

			if (_fDegree <= -180.f)
			{
				Object::ShakingEffect(1.5f, 2000.f, 1.f);
				m_fSpeed = 1.5f;
				m_eState = StickyState::Cycle2;
				m_fAimTime = 0.f;
			}
		}
	}
}

void GolemKingATK3Script::Cycle2()
{
	if (SwingDir::Left == m_eSwing)
	{
		m_fAimTime += DT;
		if (m_fAimTime <= 1.f)
		{
			return;
		}

		m_fLaunchRadian -= DT * m_fSpeed;
		m_fSpeed += DT * 0.03f;

		float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

		float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
		float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

		// Fist
		Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistPos = _pFistTr->GetRelativePosition();

		_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree));

		// FistModule1
		Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

		_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
		_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

		_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		// FistModule2
		Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
		_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

		_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		if (_fDegree <= -180.f)
		{
			Object::ShakingEffect(1.5f, 2000.f, 1.f);
			m_fSpeed = 1.5f;
			m_fAimTime = 0.f;
			m_eState = StickyState::End;
		}
	}
	else
	{
		m_fAimTime += DT;
		if (m_fAimTime <= 1.f)
		{
			return;
		}

		m_fLaunchRadian += DT * m_fSpeed;
		m_fSpeed += DT * 0.03f;

		float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

		float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
		float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

		// Fist
		Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistPos = _pFistTr->GetRelativePosition();

		_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree));

		// FistModule1
		Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

		_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
		_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

		_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		// FistModule2
		Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
		_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

		_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		if (_fDegree >= 0.f)
		{
			Object::ShakingEffect(1.5f, 2000.f, 1.f);
			m_fSpeed = 1.5f;
			m_fAimTime = 0.f;
			m_eState = StickyState::End;
		}
	}
}

void GolemKingATK3Script::Launch()
{
	if (L"Boss_Golem_Attack_StickyLaunch_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_StickyLaunch_Anim", FALSE);
	}

	if (L"Boss_Golem_Attack_StickyLaunch_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		m_fLaunchRadius += DT * 3000.f;
		float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

		float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
		float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

		// Fist
		Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistPos = _pFistTr->GetRelativePosition();

		_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));

		// FistModule1
		Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

		_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
		_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

		_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		// FistModule2
		Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
		_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

		_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr2->SetRelativeScale(Vec4(50.f, m_fLaunchRadius, _fDegree, 1.f));
		_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		if (m_fLaunchRadius >= 1400.f)
		{
			Object::ShakingEffect(1.5f, 2000.f, 1.f);
			m_eState = StickyState::Cycle1;
		}
	}
}

void GolemKingATK3Script::End()
{
	if (m_fLaunchRadius >= 120.f)
	{
		m_fLaunchRadius -= DT * 3000.f;
		float _fDegree = XMConvertToDegrees(m_fLaunchRadian);

		float _dirX = -89.f + cosf(m_fLaunchRadian) * m_fLaunchRadius;
		float _dirY = 460.f + sinf(m_fLaunchRadian) * m_fLaunchRadius;

		// Fist
		Transform* _pFistTr = m_pFist->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistPos = _pFistTr->GetRelativePosition();

		_pFistTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));

		// FistModule1
		Transform* _pFistModuleTr = m_pFistModule->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vFistModulePos = _pFistModuleTr->GetRelativePosition();

		_dirX = -89.f + cosf(m_fLaunchRadian) * 120.f;
		_dirY = 460.f + sinf(m_fLaunchRadian) * 120.f;

		_pFistModuleTr->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));

		// FistModule2
		Transform* _pFistModuleTr2 = m_pFistModule2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		_dirX = _vFistModulePos.x + cosf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);
		_dirY = _vFistModulePos.y + sinf(m_fLaunchRadian) * (m_fLaunchRadius / 2.f);

		_pFistModuleTr2->SetRelativePosition(Vec4(_dirX, _dirY, 400.f, 0.f));
		_pFistModuleTr2->SetRelativeScale(Vec4(50.f, m_fLaunchRadius, _fDegree, 1.f));
		_pFistModuleTr2->SetRelativeRotation(Vec3(0.f, 0.f, _fDegree + 90.f));
	}
	else
	{
		if (L"Boss_Golem_Attack_StickyEnd_Anim" != GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
		{
			GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Attack_StickyEnd_Anim", FALSE);
			m_pFist->Enabled(FALSE);
			m_pFistModule->Enabled(FALSE);
			m_pFistModule2->Enabled(FALSE);
		}

		if (L"Boss_Golem_Attack_StickyEnd_Anim" == GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
			GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
		{
			m_eState = StickyState::Idle;
			GetOwner()->GetScript<GolemKingScript>()->SetPattern(Pattern::Wave);
			GetOwner()->GetScript<GolemKingScript>()->SetState(BossState::Attack);
		}
	}
}