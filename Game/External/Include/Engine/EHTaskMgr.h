#pragma once

class TaskMgr
	:public Singleton<TaskMgr>
{
	Single(TaskMgr)
private:
	vector<EHTask>	m_Tasks;

public:
	void Update();

	void AddTask(const EHTask& _task)
	{
		m_Tasks.push_back(_task);
	}
};

