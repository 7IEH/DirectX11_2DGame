#include "pch.h"
#include "EHTimeMgr.h"

float TimeMgr::m_fDeltaTime = 0.f;

TimeMgr::TimeMgr()
	:m_prevCounter{}
	,m_curCounter{}
	,m_Frequency{}
{

}

TimeMgr::~TimeMgr()
{

}

void TimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_prevCounter);
}

void TimeMgr::Progress()
{
	QueryPerformanceCounter(&m_curCounter);

	float diff = static_cast<float>(m_curCounter.QuadPart) - static_cast<float>(m_prevCounter.QuadPart);

	m_fDeltaTime = diff / static_cast<float>(m_Frequency.QuadPart);

	m_prevCounter = m_curCounter;
}
