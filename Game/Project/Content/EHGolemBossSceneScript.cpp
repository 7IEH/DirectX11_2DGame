#include "pch.h"
#include "EHGolemBossSceneScript.h"

#include <EHLevelMgr.h>
#include <EHTimeMgr.h>
#include <EHAnimation2D.h>

#include "EHGolemKingScript.h"
#include "EHBossUIScript.h"

GolemBossSceneScript::GolemBossSceneScript()
	:m_eSceneState(SceneState::Start)
	, m_pGolemBoss(nullptr)
	, m_pPlayer(nullptr)
	, m_bIntro(TRUE)
	, m_bIntro2(TRUE)
	, m_bIntro3(TRUE)
	, m_bEnd(FALSE)
	, m_pNameText(nullptr)
	, m_pScroll(nullptr)
	, m_pBossDoor(nullptr)
	, m_pPostProcessing(nullptr)
	, m_pCamera(nullptr)
	, m_fTime(0.f)
	, m_pHealthBar(nullptr)
	, m_pHealthBarDeco(nullptr)
{
	SetName(L"GolemBossSceneScript");
}

GolemBossSceneScript::~GolemBossSceneScript()
{
}

void GolemBossSceneScript::Start()
{
	FIND_OBJECT(L"MainLight")->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));

	m_pGolemBoss = FIND_OBJECT(L"GolemBoss");
	m_pPlayer = FIND_OBJECT(L"Player");
	m_pNameText = FIND_OBJECT(L"GUI_BossNameUI");
	m_pScroll = FIND_OBJECT(L"GUI_GolemDungeon_Scroll");
	m_pBossDoor = FIND_OBJECT(L"Boss_Door_Bottom");
	m_pPostProcessing = FIND_OBJECT(L"Object_Mozaic_Panel");
	m_pCamera = FIND_OBJECT(L"MainCamera");
	m_pHealthBar = FIND_OBJECT(L"GUI_Boss_HealthBar");
	m_pHealthBarDeco = FIND_OBJECT(L"GUI_Boss_HealthBarDeco");

	assert(m_pGolemBoss);
	assert(m_pPlayer);
	assert(m_pNameText);
	assert(m_pScroll);
	assert(m_pBossDoor);
	assert(m_pPostProcessing);
	assert(m_pHealthBar);
	assert(m_pHealthBarDeco);

	m_pNameText->Enabled(FALSE);

	m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(0.f, -600.f, -600.f, 1.f));

	m_pBossDoor->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Golem_BossDoor_Close_Anim", FALSE);
	m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->SetCurAnimation2D(L"Boss_Golem_WakeUp_Anim");

	m_pCamera->GetScript<CameraTargetScript>()->SetTarget(m_pPlayer);

	m_bIntro = TRUE;
	m_bIntro2 = TRUE;
	m_bIntro3 = TRUE;

	Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_boss_door_closing.wav", TRUE, 0.5f);
}

void GolemBossSceneScript::Update()
{
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::ENEMY_PROJECTILE);
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::MONSTER, LAYER_TYPE::PLAYER_PROJECTILE);
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK, LAYER_TYPE::PLAYER);

	switch (m_eSceneState)
	{
	case SceneState::Start:
	{
		Enter();
	}
	break;
	case SceneState::Intro:
	{
		Intro();
	}
	break;
	case SceneState::Idle:
	{
		Idle();
	}
	break;
	case SceneState::End:
	{
		End();
	}
	break;
	default:
		break;
	}
}

void GolemBossSceneScript::LateUpdate()
{
}

void GolemBossSceneScript::Enter()
{
	Vec4 _vPos = m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	if (_vPos.y >= -175.f)
	{
		m_eSceneState = SceneState::Intro;
		m_pCamera->GetScript<CameraTargetScript>()->SetTarget(m_pGolemBoss);
		m_pCamera->GetScript<CameraTargetScript>()->SetCamSpeed(10.f);
	}
}

void GolemBossSceneScript::Intro()
{
	// Camera Move
	Vec4 _vPos = m_pCamera->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	m_fTime += DT;

	if (m_fTime >= 2.f && m_bIntro)
	{
		Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_king_golem_awake.wav", TRUE, 0.4f);
		m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_WakeUp_Anim", FALSE);
		Object::ShakingEffect(1.f, 800.f, 4.f);
		m_fTime = 0.f;
		m_bIntro = FALSE;
	}

	if (m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish() && m_bIntro2)
	{
		m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Boss_Golem_Idle_Arm_Anim");

		m_pPostProcessing->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 1);

		Object::Play2DSound(L"\\resource\\Audio\\gui_dungeon_scroll.wav", TRUE, 0.4f);
		m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"GUI_GolemDungeon_Scroll_Open_Anim", FALSE);
		m_pNameText->Enabled(TRUE);
		Object::FadeInText(m_pNameText, 2.f);
		m_pHealthBar->Enabled(TRUE);
		m_pHealthBarDeco->Enabled(TRUE);

		//Object::FadeIn(m_pHealthBar, 1.5f);
		//Object::FadeIn(m_pHealthBarDeco, 1.5f);

		m_bIntro2 = FALSE;

		Object::Play2DBGM(L"\\resource\\Audio\\golem_boss_track.wav", 0.5f);
	}

	if (!m_bIntro2)
	{
		if (L"GUI_GolemDungeon_Scroll_Open_Anim" == m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName()
			&& m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
		{
			m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"GUI_GolemDungeon_Scroll_Close_Anim", FALSE);
		}

		Transform* _pScrollTr = m_pScroll->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vScrollPos = _pScrollTr->GetRelativePosition();

		if (L"GUI_GolemDungeon_Scroll_Close_Anim" == m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
			m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish() && m_bIntro3)
		{
			float _vTempPos = _vScrollPos.y + 100.f;
			Object::MoveUp(m_pScroll, _vTempPos, 900.f);

			m_bIntro3 = FALSE;
		}

		if (_vScrollPos.y >= -101.f)
		{
			Object::MoveDown(m_pScroll, -600.f, 2000.f);

			m_eSceneState = SceneState::Idle;
			m_pCamera->GetScript<CameraTargetScript>()->SetTarget(m_pPlayer);
			m_pCamera->GetScript<CameraTargetScript>()->SetCamSpeed(2.f);
			m_pPostProcessing->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 0);
			m_pGolemBoss->GetScript<GolemKingScript>()->SetState(BossState::Idle);
		}
	}
}

void GolemBossSceneScript::Idle()
{
	if (BossState::Dead == m_pGolemBoss->GetScript<GolemKingScript>()->Getstate() && !m_bEnd)
	{
		m_pCamera->GetScript<CameraTargetScript>()->SetTarget(m_pGolemBoss);
		m_bEnd = TRUE;
		Object::FadeOutText(m_pNameText, 1.5f);
		m_pGolemBoss->GetScript<BossUIScript>()->SetDead(TRUE);
		m_eSceneState = SceneState::End;
	}
}

void GolemBossSceneScript::End()
{
	if (m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish() &&
		L"Boss_Golem_Dead_Anim2" == m_pGolemBoss->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
	{
		m_pCamera->GetScript<CameraTargetScript>()->SetTarget(m_pPlayer);
	}

	// Boss »óÀÚ
}