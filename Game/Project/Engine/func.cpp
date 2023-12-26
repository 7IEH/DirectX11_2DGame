#include "pch.h"

#include "EHTaskMgr.h"

void Object::Instantiate(GameObject* _target, int _LayerIdx)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::CREATE_OBJECT;
	_task.Param_1 = (UINT_PTR)_LayerIdx;
	_task.Param_2 = (UINT_PTR)_target;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::Destroy(GameObject* _target)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::DELETE_OBJECT;
	_task.Param_1 = (UINT_PTR)_target;
	TaskMgr::GetInst()->AddTask(_task);
}
