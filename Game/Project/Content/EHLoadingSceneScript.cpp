#include "pch.h"
#include "EHLoadingSceneScript.h"

#include <EHTimeMgr.h>
#include <EHLevelMgr.h>

extern wstring e_sNxtScene;

LoadingSceneScript::LoadingSceneScript()
	:m_fAccTime(0.f)
	, m_pLoadingUI(nullptr)
{
	SetName(L"LoadingSceneScript");
}

LoadingSceneScript::~LoadingSceneScript()
{
}

void LoadingSceneScript::Start()
{
	m_pLoadingUI = FIND_OBJECT(L"UI_Loading_Object");

	assert(m_pLoadingUI);

	m_pLoadingUI->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"UI_Loading_Anim", TRUE);

	Object::Stop2DSound(L"\\resource\\Audio\\will_step_town_dirt.wav");
	Object::Stop2DSound(L"\\resource\\Audio\\will_step_golem_dungeon.wav");
}

void LoadingSceneScript::Update()
{
	m_fAccTime += DT;
	if (m_fAccTime >= 15.f)
	{
		LevelMgr::GetInst()->SelectLevel(e_sNxtScene);
		m_fAccTime = 0.f;
	}
}