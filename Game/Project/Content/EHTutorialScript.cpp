#include "pch.h"
#include "EHTutorialScript.h"

#include <EHLevelMgr.h>
#include <EHTimeMgr.h>
#include <EHKeyMgr.h>
#include <EHAnimation2D.h>
#include "EHBabySlimeScript.h"
#include "EHTangleScript.h"
#include "EHGolemButlerScript.h"
#include "EHGolemTurretScript.h"

#include "EHTriggerScript.h"

TutorialScript::TutorialScript()
	: m_pIntroObject(nullptr)
	, m_pFadeObject(nullptr)
	, m_pFadeObject2(nullptr)
	, m_pContinueButtonObject(nullptr)
	, m_pLoadingObject(nullptr)
	, m_pPlayer(nullptr)
	, m_pIntroObjectBG(nullptr)
	, m_pTutorialScroll(nullptr)
	, m_bIntro(TRUE)
	, m_bFirst(TRUE)
	, m_bTrigger1(FALSE)
	, m_bTutorialAwake(FALSE)
	, m_bLoading(FALSE)
	, m_fAcctime(0.f)
	, m_bSkip(FALSE)
	, m_bFirstScene(FALSE)
	, m_bStart(FALSE)
	, m_mDoorObjects{}
	, m_iCurRoom(0)
	, m_pButler(nullptr)
	, m_vTurret{}
	, m_vSlime{}
	, m_pTangle(nullptr)
	, m_bSecond(FALSE)
	, m_pUICamera(nullptr)
	, m_bFinsihed(FALSE)
{
	SetName(L"TutorialScript");

}

TutorialScript::~TutorialScript()
{
}

void TutorialScript::Awake()
{
	FIND_OBJECT(L"MainLight")->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));

	if (nullptr != m_pIntroObject)
		return;

	m_pLight = FIND_OBJECT(L"MainLight");
	m_pIntroText = FIND_OBJECT(L"IntroText");
	m_pIntroObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"IntroObject");
	m_pIntroObjectBG = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"IntroBG");
	m_pFadeObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"FadeObject");
	m_pFadeObject2 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"FadeObject2");
	m_pContinueButtonObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"ContinueButton");

	m_pTutorialScroll = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Tutorial_Scroll");
	m_pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");

	m_pButler = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room4_Butler");
	
	m_pTutorialChest = FIND_OBJECT(L"Object_Tutorial_Chest");

	m_vTurret.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room5_Turret1"));
	m_vTurret.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room5_Turret2"));
	m_vTurret.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room5_Turret3"));
	m_vTurret.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room5_Turret4"));

	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime1"));
	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime2"));
	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime3"));
	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime4"));
	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime5"));
	m_vSlime.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room6_Slime6"));

	m_pTangle = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room7_Tangle1");

	for (int i = 0;i < 8;i++)
	{
		m_vTutorialBG.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_BG_Tutorial_Scene" + std::to_wstring(i + 1)));
	}

	assert(m_pIntroObjectBG);
	assert(m_pFadeObject);
	assert(m_pFadeObject2);
	assert(m_pIntroObject);
	assert(m_pContinueButtonObject);
	assert(m_pPlayer);
	assert(m_pTutorialScroll);
	assert(m_pButler);
	assert(m_pTangle);
	assert(m_pLight);
	assert(m_pTutorialChest);

	for (auto _obj : m_vTurret)
	{
		assert(_obj);
	}

	for (auto _obj : m_vSlime)
	{
		assert(_obj);
	}

	m_pTutorialScroll->Enabled(FALSE);

	for (int i = 0;i < 8;i++)
	{
		assert(m_vTutorialBG[i]);
	}

	for (int i = 0;i < 8;i++)
	{
		m_vTutorialBG[i]->Enabled(FALSE);
	}

	for (int i = 1;i <= 8;i++)
	{
		m_mDoorObjects.insert(make_pair(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Left",
			LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Left")));

		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Left")->
			GetScript<TriggerScript>(L"TriggerScript")->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Left")->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);

		if (i == 8)
			continue;

		m_mDoorObjects.insert(make_pair(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Right",
			LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Right")));

		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Right")->
			GetScript<TriggerScript>(L"TriggerScript")->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Structure_Tutorial_Room" + std::to_wstring(i) + L"_Door_Right")->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);
	}

	m_pUICamera = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UICamera");
	m_pUICamera->GetComponent<Camera>(COMPONENT_TYPE::CAMERA)->LayerVisibleSet(LAYER_TYPE::UI, FALSE);

	Object::Play2DBGM(L"\\resource\\Audio\\intro.wav", 0.5f);

	m_pTutorialScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->SetCurAnimation2D(L"Object_Tutorial_Scroll_Anim");
	m_pTutorialChest->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->SetCurAnimation2D(L"Structure_Chest_Iron_Unlocking_Anim");
	m_pTutorialChest->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

	m_pTutorialChest->GetScript<TriggerScript>()->SetTriggerType(TRIGGER_TYPE::CHEST_TRIGGER);
	m_pTutorialChest->GetScript<TriggerScript>()->SetChestItem(1, 10, 1, 0);
	m_pTutorialChest->GetScript<TriggerScript>()->SetChestItem(2, 1, 2, 3);
}

void TutorialScript::Update()
{
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::MONSTER, true);
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TRIGGER, true);

	if (nullptr == m_pIntroObject)
		return;

	Animator2D* _pAnimator = m_pIntroObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	if (m_bSkip)
	{
		if (m_bIntro)
		{
			if (m_bFirst)
			{
				m_fAcctime += DT;
				if (m_fAcctime >= 2.f)
				{
					Object::FadeOutText(m_pIntroText, 1.5f);
					m_fAcctime = 0.f;
					m_bFirst = FALSE;
					m_bSecond = TRUE;
				}
			}

			if (m_bSecond)
			{
				m_fAcctime += DT;
				if (m_fAcctime >= 2.f)
				{
					Object::FadeOut(m_pContinueButtonObject, 0.f);
					Object::FadeOut(m_pFadeObject, 0.f);
					_pAnimator->Play(L"IntroSceneAnim1-1", FALSE);
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(-140.f, -350.f, 0.f, 1.f));
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"어느 날 밤, 호화로운 보물과 무시무시한 괴물로 가득한 미로가 이 땅에 모습을 드러냈다.");
					m_bSecond = FALSE;
					m_fAcctime = 0.f;
				}
			}

			if (nullptr == _pAnimator->GetCurAnimation2D())
				return;

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-1")
			{
				_pAnimator->Play(L"IntroSceneAnim1-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-2")
			{
				_pAnimator->Play(L"IntroSceneAnim1-3", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"사람들은 낯선 땅에 세워진, 끊임없이 변화하는 기이한 유적을 던전이라 불렀다.");
					_pAnimator->Play(L"IntroSceneAnim2-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim2-1")
			{
				_pAnimator->Play(L"IntroSceneAnim2-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim2-2")
			{
				_pAnimator->Play(L"IntroSceneAnim2-3", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim2-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"머지 않아 던전 근처에는 신기한 마을이 터를 잡았다. 사람들은 마을을 리노카라 불렀다.");
					_pAnimator->Play(L"IntroSceneAnim3-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim3-1")
			{
				_pAnimator->Play(L"IntroSceneAnim3-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim3-2")
			{
				_pAnimator->Play(L"IntroSceneAnim3-3", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim3-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"정착민들 중에서는 두 집단이 가장 두각을 나타냈다. 영웅과 상인, 영광과 부.");
					_pAnimator->Play(L"IntroSceneAnim4-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim4-1")
			{
				_pAnimator->Play(L"IntroSceneAnim4-2", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim4-2")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"그러나 던전은 너무나 위험했다. 던전의 깊은 곳에서 너무나 많은 사람들이 죽자, 던전은 폐쇄되었다. ");
					_pAnimator->Play(L"IntroSceneAnim5-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim5-1")
			{
				_pAnimator->Play(L"IntroSceneAnim5-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim5-2")
			{
				_pAnimator->Play(L"IntroSceneAnim5-3", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim5-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"마을 주민의 삶은 점점 어려워졌고, 문라이터라는 오래된 상점을 운영하는 윌의 삶은 특히 더 어려워졌다.");
					_pAnimator->Play(L"IntroSceneAnim6-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim6-1")
			{
				_pAnimator->Play(L"IntroSceneAnim6-2", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim6-2")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"비밀에 싸인 5번째 문을 열겠다는 오랜 꿈을 간직한 채로.");
					_pAnimator->Play(L"IntroSceneAnim7-1", FALSE);
					Object::FadeOut(m_pFadeObject, 1.5f);
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim7-1")
			{
				_pAnimator->Play(L"IntroSceneAnim7-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim7-2")
			{
				_pAnimator->Play(L"IntroSceneAnim7-3", FALSE);
				Object::FadeOutText(m_pIntroText, 1.5f);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim7-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					m_bIntro = FALSE;
					Object::FadeIn(m_pContinueButtonObject, 1.5f);
					Object::FadeInText(m_pIntroText, 1.5f);
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetFontSize(18.f);
					m_pIntroText->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(-600.f, -356.f, -5.f, 0.f));
					m_pIntroText->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"계속하기");
					_pAnimator->Play(L"IntroSceneAnim7-1", FALSE);
					Object::Stop2DSound(L"\\resource\\Audio\\intro.wav");

					m_pFadeObject->Enabled(FALSE);
					m_pFadeObject2->Enabled(FALSE);
					m_pIntroObject->Enabled(FALSE);
					m_bTutorialAwake = TRUE;
					m_fAcctime = 0.f;
					m_bTrigger1 = FALSE;
				}
				else if (!m_bTrigger1)
				{
					Object::FadeIn(m_pFadeObject, 1.5f);
					m_bTrigger1 = TRUE;
				}
			}
		}
	}
	else
	{
		if (!m_bTrigger1)
		{
			Object::FadeIn(m_pContinueButtonObject, 1.5f);
			m_pFadeObject->Enabled(TRUE);
			m_pIntroObject->Enabled(FALSE);
			m_bTutorialAwake = TRUE;
			m_fAcctime = 0.f;
			m_bTrigger1 = TRUE;
		}
	}

	if (m_bTutorialAwake)
	{
		// 튜토리얼 시작
		if (KEY_TAP(KEY::J))
		{
			m_pContinueButtonObject->Enabled(FALSE);
			m_pIntroObjectBG->Enabled(FALSE);
			m_pFadeObject->Enabled(FALSE);
			m_pFadeObject2->Enabled(FALSE);
			Object::FadeOutText(m_pIntroText, 1.5f);
			Object::FadeInLightAmbient(m_pLight, 1.5f);
			m_pTutorialScroll->Enabled(TRUE);
			m_bLoading = TRUE;

			for (int i = 0;i < 8;i++)
			{
				m_vTutorialBG[i]->Enabled(TRUE);
			}
			// 캐릭터 Enabled
		}

		if (m_bLoading)
		{
			m_bLoading = FALSE;
			m_bTutorialAwake = FALSE;
			m_bFirstScene = TRUE;

			m_fAcctime = 0.f;
		}
	}

	if (m_bFirstScene)
	{
		Animator2D* _anim = m_pPlayer->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
		if (L"FSM_Player_Tutorial_Scene0_Anim" != _anim->GetCurAnimation2D()->GetName())
		{
			m_pPlayer->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"FSM_Player_Tutorial_Scene0_Anim", FALSE);
			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->SetState(State::Scene);

			m_mDoorObjects.find(L"Structure_Tutorial_Room1_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->GetEnabled();

			m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativeScale(Vec4(90.f, 90.f, 1.f, 1.f));
		}

		if (m_pPlayer->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
		{
			m_pPlayer->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativeScale(Vec4(500.f, 500.f, 1.f, 1.f));

			m_bFirstScene = FALSE;
			m_pTutorialScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Tutorial_Scroll_Anim", FALSE);
			Object::Play2DSound(L"\\resource\\Audio\\tutorial_scroll.wav", TRUE, 0.5f);

			Object::Play2DBGM(L"\\resource\\Audio\\golem_dungeon_floor_variation_1.wav", 0.5f);
			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_main_ambient.wav", FALSE, 0.2f);

			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->SetDirection(Dir::UP);
			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->IdleAnim();
			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->SetState(State::Idle);

			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav",TRUE,0.5f);
			m_mDoorObjects.find(L"Structure_Tutorial_Room1_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav", FALSE, 0.5f);
			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_closing.wav", FALSE, 0.5f);

			m_bStart = TRUE;
			m_pUICamera->GetComponent<Camera>(COMPONENT_TYPE::CAMERA)->LayerVisibleSet(LAYER_TYPE::UI, TRUE);
		}
	}

	if (m_bStart)
	{
		RoomOpen();
	}
}

void TutorialScript::LateUpdate()
{
}

void TutorialScript::RoomOpen()
{
	PlayerScript* _pScript = m_pPlayer->GetScript<PlayerScript>(L"PlayerScript");
	if (State::CameraMove == _pScript->m_eState)
	{
		switch (_pScript->m_eDir)
		{
		case Dir::LEFT:
		{
			m_iCurRoom -= 1;
		}
		break;
		case Dir::RIGHT:
		{
			m_iCurRoom += 1;
		}
		break;
		default:
			break;
		}
	}

	if (m_iCurRoom == 1)
	{
		if (m_mDoorObjects.find(L"Structure_Tutorial_Room2_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
		{
			return;
		}

		m_mDoorObjects.find(L"Structure_Tutorial_Room2_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);


		m_mDoorObjects.find(L"Structure_Tutorial_Room2_Door_Left")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);
		Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav", TRUE, 0.5f);
	}
	else if (m_iCurRoom == 2)
	{
		if (m_mDoorObjects.find(L"Structure_Tutorial_Room3_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
		{
			return;
		}

		m_mDoorObjects.find(L"Structure_Tutorial_Room3_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

		m_mDoorObjects.find(L"Structure_Tutorial_Room3_Door_Left")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);
		Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav", TRUE, 0.5f);
	}
	else if (m_iCurRoom == 3)
	{
		if (m_mDoorObjects.find(L"Structure_Tutorial_Room4_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
		{
			return;
		}

		m_pButler->GetScript<GolemButlerScript>()->SetChase(TRUE);

		m_mDoorObjects.find(L"Structure_Tutorial_Room4_Door_Right")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

		m_mDoorObjects.find(L"Structure_Tutorial_Room4_Door_Left")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);
		Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav", TRUE, 0.5f);
	}
	else if (m_iCurRoom == 4)
	{
		vector<GameObject*>::iterator iter = m_vTurret.begin();

		if (State::CameraMove != _pScript->m_eState)
		{
			for (;iter != m_vTurret.end();iter++)
			{
				if ((*iter) != nullptr)
				{
					if (!(*iter)->GetDead())
					{
						(*iter)->GetScript<GolemTurretScript>()->SetChase(TRUE);
					}
				}
			}
		}

		if (State::CameraMove != _pScript->m_eState)
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room5_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
			{
				return;
			}

			m_mDoorObjects.find(L"Structure_Tutorial_Room5_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room5_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);
			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_opening.wav", TRUE, 0.5f);
		}
	}
	else if (m_iCurRoom == 5)
	{
		for (auto _pTurret : m_vTurret)
		{
			_pTurret->SetDead(TRUE);
		}

		// 공격
		bool _flag = FALSE;
		for (auto _pSlime : m_vSlime)
		{
			if (_pSlime != nullptr)
			{
				if (!_pSlime->GetDead())
				{
					_flag = TRUE;
				}
			}
		}

		if (State::CameraMove != _pScript->m_eState)
		{
			for (auto _pSlime : m_vSlime)
			{
				if (_pSlime != nullptr)
				{
					if (!_pSlime->GetDead())
					{
						_pSlime->GetScript<BabySlimeScript>()->SetChase(TRUE);
					}
				}
			}
		}

		// 닫힘
		if (_flag)
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Close_Tutorial_Anim")
			{
				return;
			}

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);
			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_closing.wav", TRUE, 0.5f);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
		}

		// 열림
		else
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
			{
				return;
			}

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Right")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room6_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_Opening.wav", TRUE, 0.5f);
		}
	}
	else if (m_iCurRoom == 6)
	{
		// 공격
		if (State::CameraMove != _pScript->m_eState)
		{
			if (m_pTangle != nullptr)
				m_pTangle->GetScript<TangleScript>()->SetChase(TRUE);
		}

		// 닫힘
		if (!m_pTangle->GetDead())
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Close_Tutorial_Anim")
			{
				return;
			}
			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_closing.wav", TRUE, 0.5f);
		}

		// 열림
		else
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
			{
				return;
			}

			m_pTutorialChest->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			Object::Play2DSound(L"\\resource\\Audio\\dungeon_chest_unlock.wav", TRUE, 0.5f);
			m_pTutorialChest->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Chest_Iron_Unlocking_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			Object::Play2DSound(L"\\resource\\Audio\\golem_dungeon_normal_door_Opening.wav", TRUE, 0.5f);
		}
	}
	else if (m_iCurRoom == 7)
	{
		/*if (m_mDoorObjects.find(L"Structure_Tutorial_Room8_Door_Left")->second->
			GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() != L"Structure_Door_Close_Tutorial_Anim")
		{
			m_mDoorObjects.find(L"Structure_Tutorial_Room8_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Close_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room8_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
		}

		if (State::Dead == _pScript->m_eState)
		{
			LevelMgr::GetInst()->SelectLevel(L"TownScene");
		}*/

		if (!m_bFinsihed)
		{
			Object::Stop2DSound(L"\\resource\\Audio\\golem_dungeon_main_ambient.wav");
			Object::Stop2DSound(L"\\resource\\Audio\\golem_dungeon_floor_variation_1.wav");

			SceneManager::SelectScene(L"TownScene");
			RecordManager::GetInst()->GetCurrentPlayerPref()->_bTutorial = 1;
			RecordManager::GetInst()->GetCurrentPlayerPref()->_ePlace = PLACE::TOWN;
			RecordManager::GetInst()->GetCurrentPlayerPref()->_iTime = 0;
			RecordManager::GetInst()->SaveFile();
			m_bFinsihed = TRUE;
		}
	}
}