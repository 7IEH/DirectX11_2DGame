#include "pch.h"
#include "EHBannerScript.h"

#include <EHLevelMgr.h>
#include <EHTimeMgr.h>

BannerScript::BannerScript()
	:m_pBannerObject(nullptr)
	, m_fAccTime(0.f)
	, m_bFirstBanner(FALSE)
{
	SetName(L"BannerScript");
}

BannerScript::~BannerScript()
{
}

void BannerScript::Start()
{
	m_pBannerObject = FIND_OBJECT(L"Object_FMOD_Banner");

	assert(m_pBannerObject);
}

void BannerScript::Update()
{
	m_fAccTime += DT;

	if (m_fAccTime >= 7.f && !m_bFirstBanner)
	{
		Object::FadeOut(m_pBannerObject, 2.5f);
		m_fAccTime = 0.f;
		m_bFirstBanner = TRUE;
	}

	if (m_fAccTime >= 3.5f && m_bFirstBanner)
	{
		LevelMgr::GetInst()->SelectLevel(L"EnterScene");
	}
}