#include "pch.h"
#include "EHTutorialScript.h"

#include <EHLevelMgr.h>
#include <EHAnimation2D.h>

TutorialScript::TutorialScript()
	: m_pIntroObject(nullptr)
	, m_bIntro(TRUE)
	, m_bFirst(TRUE)
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

	assert(m_pIntroObject);
}

void TutorialScript::Update()
{
	if (nullptr == m_pIntroObject)
		return;

	Animator2D* _pAnimator = m_pIntroObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	if (m_bIntro)
	{
		if (m_bFirst)
		{
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
			m_bIntro = FALSE;
		}
	}

}

void TutorialScript::LateUpdate()
{
}