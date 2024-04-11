#include "pch.h"
#include "EHSpawnEffectScript.h"

#include <EHTimeMgr.h>

SpawnEffectScript::SpawnEffectScript()
	:m_fDeleteTime(6.f)
	, m_fAcctime(0.f)
	, m_fFadeOut(FALSE)
{
	SetName(L"SpawnEffectScript");
}

SpawnEffectScript::~SpawnEffectScript()
{
}

void SpawnEffectScript::Update()
{
	m_fAcctime += DT;

	if (m_fFadeOut)
	{
		if (m_fDeleteTime <= m_fAcctime)
		{
			Object::Destroy(GetOwner());
		}
	}
	else
	{
		if (m_fDeleteTime <= m_fAcctime)
		{
			Object::Destroy(GetOwner());
		}
	}
}