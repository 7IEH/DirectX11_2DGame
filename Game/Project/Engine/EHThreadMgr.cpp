#include "pch.h"
#include "EHThreadMgr.h"

#include "EHMapGenerator.h"

ThreadMgr::ThreadMgr()
{

}

ThreadMgr::~ThreadMgr()
{
	vector<std::thread>::iterator iter = m_ThreadTask.begin();
	map<wstring, HANDLE>::iterator iter2 = m_EventPoint.begin();

	for (;iter2 != m_EventPoint.end();iter2++)
	{
		::SetEvent((*iter2).second);
	}

	for (;iter != m_ThreadTask.end();iter++)
	{
		(*iter).join();
	}

	ReleaseVector(m_Release);
}

void ThreadMgr::Awake()
{
	// Thread Fucntional Initialize
	MapGenerator* _gen = new MapGenerator();
	HANDLE _handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ThreadTask.push_back(std::thread(&MapGenerator::Awake,_gen));
	_gen->SetTrigger(_handle);
	
	m_EventPoint.insert({ L"MapGenerator1",_handle });
	m_Release.push_back(_gen);
}

void ThreadMgr::StartThread(const wstring& _name)
{
	HANDLE _handle = FindThread(_name);
	if (_handle == nullptr)
		return;
	
	::SetEvent(_handle);
}

HANDLE ThreadMgr::FindThread(const wstring& _name)
{
	map<wstring, HANDLE>::iterator iter = m_EventPoint.find(_name);
	if (iter == m_EventPoint.end())
	{
		return nullptr;
	}

	return (*iter).second;
}