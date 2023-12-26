#include "pch.h"
#include "EHTaskMgr.h"

#include "EHLevelMgr.h"

#include "EHGameObject.h"
#include "EHLevel.h"
#include "EHComponent.h"

TaskMgr::TaskMgr()
{
}

TaskMgr::~TaskMgr()
{
}

void TaskMgr::Update()
{
	for (size_t i = 0;i < m_Tasks.size();i++)
	{
		switch (m_Tasks[i]._Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			int LayerIdx = (int)m_Tasks[i].Param_1;
			GameObject* _obj = (GameObject*)m_Tasks[i].Param_2;

			Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();
			_curLevel->AddObject(_obj, LAYER_TYPE(LayerIdx));

			/*if(LEVEL_STATE::PLAY == _curLevel->GetState())
			{
				_obj->begin();
			}*/
		}
			break;
		case TASK_TYPE::DELETE_OBJECT:
			break;
		case TASK_TYPE::LEVEL_CHANGE:
			break;
		case TASK_TYPE::ADD_CHILD:
			break;
		case TASK_TYPE::DISCONNECT_PARENT:
			break;
		default:
			break;
		}
	}
}
