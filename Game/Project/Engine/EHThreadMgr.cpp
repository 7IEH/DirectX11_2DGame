#include "pch.h"
#include "EHThreadMgr.h"

#include "EHMapGenerator.h"

ThreadMgr::ThreadMgr()
{

}

ThreadMgr::~ThreadMgr()
{
	for (auto _task : m_EventPoint)
	{
		if (_task.second != nullptr)
		{
			delete _task.second;
			_task.second = nullptr;
		}
	}
}

void ThreadMgr::Awake()
{
	// Thread Fucntional Initialize
	MapGenerator* _gen = new MapGenerator();
	HANDLE _handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ThreadTask.push_back(std::thread(&MapGenerator::Awake,_gen));
	_gen->SetTrigger(_handle);

	m_EventPoint.insert({ L"MapGenerator1",_handle });
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