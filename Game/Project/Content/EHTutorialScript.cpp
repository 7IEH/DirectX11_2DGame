#include "pch.h"
#include "EHTutorialScript.h"

#include <EHLevelMgr.h>
#include <EHTimeMgr.h>
#include <EHKeyMgr.h>
#include <EHAnimation2D.h>

TutorialScript::TutorialScript()
	: m_pIntroObject(nullptr)
	, m_pFadeObject(nullptr)
	, m_pContinueButtonObject(nullptr)
	, m_pLoadingObject(nullptr)
	, m_pPlayer(nullptr)
	, m_bIntro(TRUE)
	, m_bFirst(TRUE)
	, m_bTrigger1(FALSE)
	, m_bTutorialAwake(FALSE)
	, m_bLoading(FALSE)
	, m_fAcctime(0.f)
	, m_bSkip(FALSE)
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

	for (int i = 0;i < 7;i++)
	{
		m_vTutorialBG.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"TutorialBG" + std::to_wstring(i + 1)));
	}

	assert(m_pIntroObjectBG);
	assert(m_pFadeObject);
	assert(m_pFadeObject2);
	assert(m_pIntroObject);
	assert(m_pContinueButtonObject);

	for (int i = 0;i < 7;i++)
	{
		assert(m_vTutorialBG[i]);
	}

	for (int i = 0;i < 7;i++)
	{
		m_vTutorialBG[i]->Enabled(FALSE);
	}
}

void TutorialScript::Update()
{
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

			for (int i = 0;i < 7;i++)
			{
				m_vTutorialBG[i]->Enabled(TRUE);
			}
			// 캐릭터 Enabled
			/*m_pPlayer->Enabled(TRUE);*/
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
			m_fAcctime = 0.f;
		}
	}
	// 던전 로직



}

void TutorialScript::LateUpdate()
{
}