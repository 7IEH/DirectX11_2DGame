#pragma once

struct Fade_Effect
{
	GameObject* _pObject;
	TASK_TYPE _tTask;
	float _fAcctime;
	float _fMaxTime;
};

struct Light_Effect
{
	GameObject* _pObject;
	TASK_TYPE _tTask;
	float _fRadius;
	float _fMaxRadius;
	float _fAcctime;
	float _fMaxTime;

	Vec4 _vColor;
};

struct Move_Effect
{
	GameObject* _pObject;
	TASK_TYPE  _tTask;
	float		_fDest;
	float		_fSpeed;
};

struct Shake_Effect
{
	GameObject* _pObject;
	Vec4		_vOriginPos;
	float		_fAcctime;
	float		_fTime;
	float		_fSpeed;
	float		_fDitance;
};

struct Scale_Effect
{
	GameObject* _pObject;
	TASK_TYPE	_Type;
	Vec4		_vScale;
	float		_fSpeed;
};

struct SceneChange
{
	float _fAcctime;
};

class TaskMgr
	:public Singleton<TaskMgr>
{
	Single(TaskMgr)
private:
	vector<EHTask>	m_Tasks;

	vector<Fade_Effect> m_FadeTasks;
	vector<Light_Effect> m_LightTasks;
	vector<Light_Effect> m_AmbientTasks;
	vector<Fade_Effect> m_FontTasks;
	vector<Move_Effect> m_MoveTasks;
	vector<Shake_Effect> m_ShakeTasks;
	vector<Scale_Effect> m_ScaleTasks;
	vector<SceneChange> m_SceneTasks;

public:
	void Update();

	void AddTask(const EHTask& _task)
	{
		m_Tasks.push_back(_task);
	}

private:
	void FadeUpdate();
	void LightUpdate();
	void AmbientUpdate();
	void FontUpdate();
	void MoveUpdate();
	void ShakeUpdate();
	void ScaleUpdate();
	void SceneChangeUpdate();
};

