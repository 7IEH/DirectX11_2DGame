#include "pch.h"
#include "EHGolemDungeonScript.h"

#include <EHLevelMgr.h>
#include <EHAnimation2D.h>

GolemDungeonScript::GolemDungeonScript()
	:m_pScroll(nullptr)
	, m_pPlayer(nullptr)
	, m_pIntroBG(nullptr)
	, m_pIntroText(nullptr)
	, m_pIntroDoor(nullptr)
	, m_bIntro1(TRUE)
	, m_bIntro2(TRUE)
{
	SetName(L"GolemDungeonScript");
	SetDungeonType(DungeonType::GOLEM);
}

GolemDungeonScript::~GolemDungeonScript()
{
}

void GolemDungeonScript::Start()
{
	DungeonScript::Start();

	m_pScroll = FIND_OBJECT(L"GUI_GolemDungeon_Scroll");
	m_pIntroBG = FIND_OBJECT(L"GUI_IntroBG");
	m_pIntroText = FIND_OBJECT(L"GUI_IntroText");
	m_pIntroDoor = FIND_OBJECT(L"Object_Intro_Door");

	assert(m_pScroll);
	assert(m_pIntroBG);
	assert(m_pIntroText);
	assert(m_pIntroDoor);

	m_pScroll->Enabled(FALSE);
	m_pIntroText->Enabled(FALSE);
	m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"GUI_GolemDungeon_Scroll_Open_Anim", FALSE);
	m_pIntroBG->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));
	m_pIntroDoor->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Intro_Door_Close_Anim",FALSE);
}

void GolemDungeonScript::Update()
{
	DungeonScript::Update();

	Transform* _pScrollTr = m_pScroll->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vScrollPos = _pScrollTr->GetRelativePosition();

	if (m_pIntroDoor->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish() && m_bIntro1)
	{
		m_pScroll->Enabled(TRUE);
		m_pIntroText->Enabled(TRUE);
		m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"GUI_GolemDungeon_Scroll_Open_Anim", FALSE);
		Object::FadeInText(m_pIntroText, 1.5f);
		m_bIntro1 = FALSE;
	}

	if (L"GUI_GolemDungeon_Scroll_Open_Anim" == m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
	{
		m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"GUI_GolemDungeon_Scroll_Close_Anim", FALSE);
	}

	if (L"GUI_GolemDungeon_Scroll_Close_Anim" == m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() &&
		m_pScroll->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish() && m_bIntro2)
	{
		Object::FadeOutText(m_pIntroText, 1.5f);
		//Object::FadeOut(m_pIntroBG, 1.5f);

		float _vTempPos = _vScrollPos.y + 100.f;
		Object::MoveUp(m_pScroll, _vTempPos, 900.f);

		m_bIntro2 = FALSE;
	}

	if (_vScrollPos.y >= -101.f)
	{
		Object::MoveDown(m_pScroll, -600.f, 2000.f);
	}
}

void GolemDungeonScript::LateUpdate()
{
}