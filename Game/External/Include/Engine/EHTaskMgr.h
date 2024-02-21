#pragma once

struct Fade_Effect
{
	TASK_TYPE _tTask;
	GameObject* _pObject;
	float _fAcctime;
	float _fMaxTime;
};

struct Light_Effect
{
	TASK_TYPE _tTask;
	GameObject* _pObject;
	float _fRadius;
	float _fMaxRadius;
	float _fAcctime;
	float _fMaxTime;

	Vec4 _vColor;
};

class TaskMgr
	:public Singleton<TaskMgr>
{
	Single(TaskMgr)
private:
	vector<EHTask>	m_Tasks;

	vector<Fade_Effect> m_FadeTasks;
	vector<Light_Effect> m_LightTasks;

public:
	void Update();

	void AddTask(const EHTask& _task)
	{
		m_Tasks.push_back(_task);
	}

private:
	void FadeUpdate();
	void LightUpdate();
};

