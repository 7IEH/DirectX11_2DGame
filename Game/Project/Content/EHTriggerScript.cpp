#include "pch.h"
#include "EHTriggerScript.h"

#include "EHGameObject.h"
#include "EHDebugMgr.h"

#include "EHRenderMgr.h"

#include "EHLevelMgr.h"
#include "EHRandomManager.h"

TriggerScript::TriggerScript()
	: m_eTriggerType(TRIGGER_TYPE::END)
	, m_pCam(nullptr)
	, m_vPlayerPos{}
	, m_vCamPos{}
	, m_eChestStore{}
	, m_iChestStore{}
	, m_iChestNum(0)
{
	SetName(L"TriggerScript");
}

TriggerScript::~TriggerScript()
{
}

void TriggerScript::Awake()
{
	int _flag = 0;
	if (TRIGGER_TYPE::CHEST_TRIGGER == m_eTriggerType)
	{
		for (int i = 0;i < 28;i++)
		{
			if (m_iChestNum == _flag)
				break;
			m_eChestStore[i] = ITEM::NO_ITEM;
			m_iChestStore[i] = 0;

		}
	}
}
