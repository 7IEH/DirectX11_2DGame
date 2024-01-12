#include "pch.h"
#include "EHDebugMgr.h"

DebugMgr::DebugMgr()
	:m_DebugMessage{}
{
}

DebugMgr::~DebugMgr()
{
}

void DebugMgr::LateUpdate()
{
	m_DebugMessage.clear();
}