#include "pch.h"
#include "EHPathMgr.h"

wchar_t PathMgr::m_wstrPath[255];

PathMgr::PathMgr()
{

}

PathMgr::~PathMgr()
{

}

void PathMgr::Init()
{
	GetCurrentDirectory(255, m_wstrPath);

	size_t _length = wcslen(m_wstrPath);

	for (size_t i = _length - 1;i >= 0;--i)
	{
		if (m_wstrPath[i] == '\\')
		{
			m_wstrPath[i + 1] = '\0';
			break;
		}
	}

	_length = wcslen(m_wstrPath);

	for (size_t i = _length - 2;i >= 0;--i)
	{
		if (m_wstrPath[i] == '\\')
		{
			m_wstrPath[i + 1] = '\0';
			break;
		}
	}

	wcscat_s(m_wstrPath, L"content");
}
