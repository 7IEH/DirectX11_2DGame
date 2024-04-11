#include "pch.h"
#include "EHSpawnRockScript.h"

#include <EHTimeMgr.h>
#include <EHGameObject.h>

SpawnRockScript::SpawnRockScript()
	:m_fTarget(0.f)
	, m_fAcctime(0.f)
	, m_bStart(TRUE)
	, m_bFadeOut(FALSE)
	, m_bDead(FALSE)
{
	SetName(L"SpawnRockScript");
}

SpawnRockScript::~SpawnRockScript()
{
}

void SpawnRockScript::Update()
{
	m_fAcctime += DT;
	if (m_bStart)
	{
		Object::MoveDown(GetOwner(), m_fTarget, 3000.f);
		m_bStart = FALSE;
		m_bFadeOut = TRUE;
	}

	if (GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y <= m_fTarget + 10.f)
	{
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
	}
	
	if (m_fAcctime >= 6.f && m_bFadeOut)
	{
		m_bFadeOut = FALSE;
		m_bDead = TRUE;
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
	}

	if (m_fAcctime >= 8.5f && m_bDead)
	{
		Object::Destroy(GetOwner());
		m_fAcctime = 0.f;
		m_bDead = FALSE;
	}
}