#include "pch.h"
#include "EHTimeMgr.h"
#include "EHEngine.h"

TimeMgr::TimeMgr()
	:m_prevCounter{}
	,m_curCounter{}
	,m_Frequency{}
	,m_fDeltaTime(0.f)
	,m_fTime(0.f)
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

	m_fDeltaTime = static_cast<float>(m_curCounter.QuadPart-m_prevCounter.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	m_prevCounter = m_curCounter;

	m_fTime += m_fDeltaTime;
	if (1.f <= m_fTime)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_fDeltaTime, m_iCall);
		SetWindowText(Engine::GetInst()->GetMainWind(), szText);

		m_iCall = 0;
		m_fTime = 0.f;
	}

	++m_iCall;
}
