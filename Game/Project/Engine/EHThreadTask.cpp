#include "pch.h"
#include "EHThreadTask.h"

ThreadTask::ThreadTask()
{
}

ThreadTask::~ThreadTask()
{
}

void ThreadTask::Awake()
{
	::WaitForSingleObject(m_Trigger, INFINITE);
}