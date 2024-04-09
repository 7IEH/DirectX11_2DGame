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
{
	SetName(L"TutorialScript");

}

TutorialScript::~TutorialScript()
{
}

void TutorialScript::Awake()
{
	if (nullptr != m_pIntroObject)
		return;

	m_pIntroObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"IntroObject");
	m_pIntroObjectBG = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"IntroBG");
	m_pFadeObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"FadeObject");
	m_pFadeObject2 = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"FadeObject2");
	m_pContinueButtonObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"ContinueButton");

	m_pTutorialScroll = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Tutorial_Scroll");
	m_pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");

	m_pButler = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Enemy_Tutorial_Room4_Butler");

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
}

void TutorialScript::Update()
{
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::MONSTER, true);

	if (nullptr == m_pIntroObject)
		return;

	Animator2D* _pAnimator = m_pIntroObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	if (m_bSkip)
	{
		if (m_bIntro)
		{
			if (m_bFirst)
			{
				Object::FadeOut(m_pContinueButtonObject, 0.f);
				Object::FadeOut(m_pFadeObject, 0.f);
				_pAnimator->Play(L"IntroSceneAnim1-1", FALSE);
				m_bFirst = FALSE;
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-1")
			{
				_pAnimator->Play(L"IntroSceneAnim1-2", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-2")
			{
				_pAnimator->Play(L"IntroSceneAnim1-3", FALSE);
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim1-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim2-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim3-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim4-2")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim5-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim6-2")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
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
			}

			if (_pAnimator->GetCurAnimation2D()->IsFinish() && _pAnimator->GetCurAnimation2D()->GetName() == L"IntroSceneAnim7-3")
			{
				m_fAcctime += DT;
				if (m_fAcctime > 1.5f)
				{
					m_bIntro = FALSE;
					Object::FadeIn(m_pContinueButtonObject, 1.5f);
					m_pFadeObject->Enabled(TRUE);
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

			m_bLoading = TRUE;

			// 로딩 오브젝트 실시
			/*m_pLoadingObject->Enabled(TRUE);
			m_pLoadingObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"LoadingAnim");*/

			for (int i = 0;i < 8;i++)
			{
				m_vTutorialBG[i]->Enabled(TRUE);
			}
			// 캐릭터 Enabled
		}

		// 튜토리얼 시작 한 후 로딩 후 페이드 인
		if (m_bLoading)
		{
			m_fAcctime += DT;
		}

		if (m_fAcctime >= 3.f)
		{
			Object::FadeOut(m_pFadeObject2, 3.f);
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
			m_pTutorialScroll->Enabled(TRUE);
			m_pTutorialScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Tutorial_Scroll_Anim", FALSE);

			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->SetDirection(Dir::UP);
			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->IdleAnim();
			m_pPlayer->GetScript<PlayerScript>(L"PlayerScript")->SetState(State::Idle);

			m_mDoorObjects.find(L"Structure_Tutorial_Room1_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);
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
		}
	}
	else if (m_iCurRoom == 5)
	{
		for (auto _pTurret:m_vTurret)
		{
			_pTurret->SetDead(TRUE);
		}

		// 공격
		bool _flag = FALSE;
		for (auto _pSlime :m_vSlime)
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
		}

		// 열림
		else
		{
			if (m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() == L"Structure_Door_Open_Tutorial_Anim")
			{
				return;
			}

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Structure_Door_Open_Tutorial_Anim", FALSE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Right")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);

			m_mDoorObjects.find(L"Structure_Tutorial_Room7_Door_Left")->second->
				GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
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

		LevelMgr::GetInst()->SelectLevel(L"TownScene");
		RecordManager::GetInst()->GetCurrentPlayerPref()->_bTutorial = 1;
		RecordManager::GetInst()->GetCurrentPlayerPref()->_ePlace = PLACE::TOWN;
		RecordManager::GetInst()->GetCurrentPlayerPref()->_iTime = 1;

		RecordManager::GetInst()->SaveFile();
	}
}