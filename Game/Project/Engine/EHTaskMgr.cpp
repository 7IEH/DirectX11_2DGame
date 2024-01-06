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
		{
			GameObject* _obj = (GameObject*)m_Tasks[i].Param_1;


			queue<GameObject*>_q;
			_q.push(_obj);

			while (!_q.empty())
			{
				GameObject* _temp = _q.front(); _q.pop();

				_temp->SetDead(true);

				for (size_t i = 0;i < _temp->m_Childs.size();i++)
				{
					_q.push(_temp->m_Childs[i]);
				}
			}
		}
			break;
		case TASK_TYPE::LEVEL_CHANGE:
		{
			Level* _curLevel = (Level*)m_Tasks[i].Param_1;

			LevelMgr::GetInst()->SelectLevel(_curLevel);
		}
			break;
		case TASK_TYPE::ADD_CHILD:
			break;
		case TASK_TYPE::DISCONNECT_PARENT:
			break;
		default:
			break;
		}
	}
	m_Tasks.clear();
}
