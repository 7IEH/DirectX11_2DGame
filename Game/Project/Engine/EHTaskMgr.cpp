#include "pch.h"
#include "EHTaskMgr.h"

#include "EHLevelMgr.h"

#include "EHGameObject.h"
#include "EHLevel.h"
#include "EHComponent.h"

#include "EHTimeMgr.h"

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

		/*****************************
		|	SPECIAL EFFECT
		*****************************/
		case TASK_TYPE::ADD_CHILD:
			break;
		case TASK_TYPE::FADE_IN:
		{
			Fade_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_IN;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;

			_effect._pObject->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));
			m_FadeTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_OUT:
		{
			Fade_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_OUT;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			m_FadeTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_IN_LIGHT_COLOR:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_IN_LIGHT_COLOR;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._vColor = _effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetColor();

			_effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetColor(Vec4(0.f, 0.f, 0.f, 1.f));
			m_LightTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_OUT_LIGHT_COLOR:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_OUT_LIGHT_COLOR;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._vColor = _effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetColor();
			m_LightTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_IN_LIGHT_RADIUS:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_IN_LIGHT_RADIUS;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._fMaxRadius = (float)m_Tasks[i].Param_3;

			_effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetRadius(0.f);
			m_LightTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_OUT_LIGHT_RADIUS:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_OUT_LIGHT_RADIUS;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._fMaxRadius = (float)m_Tasks[i].Param_3;
			_effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetRadius(_effect._fMaxRadius);
			m_LightTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::DISCONNECT_PARENT:
			break;
		default:
			break;
		}
	}
	m_Tasks.clear();

	FadeUpdate();
	LightUpdate();
}

void TaskMgr::FadeUpdate()
{
	vector<Fade_Effect>::iterator iter = m_FadeTasks.begin();
	for (;iter != m_FadeTasks.end();)
	{
		Ptr<Material> _mat = iter->_pObject->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial();

		float _fAlpha = 0.f;

		float _dt = DT;
		if (_dt >= 1.f)
		{
			return;
		}
		iter->_fAcctime += _dt;

		float _fRatio = iter->_fAcctime / iter->_fMaxTime;
		if (_fRatio >= 1.f)
		{
			_fRatio = 1.f;
		}

		if (iter->_tTask == TASK_TYPE::FADE_IN)
		{
			_fAlpha = 0.f;
			_fAlpha = _fRatio;
		}
		else
		{
			_fAlpha = 1.f;
			_fAlpha = (1.f - _fRatio);
		}

		_mat->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, _fAlpha));

		if (_fRatio >= 1.f)
		{
			iter = m_FadeTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskMgr::LightUpdate()
{
	vector<Light_Effect>::iterator iter = m_LightTasks.begin();
	for (;iter != m_LightTasks.end();)
	{
		Vec4 _lightColor = Vec4(0.f, 0.f, 0.f, 1.f);
		float _radius = 0.f;
		float _dt = DT;
		if (_dt >= 1.f)
		{
			return;
		}
		iter->_fAcctime += _dt;

		float _fRatio = iter->_fAcctime / iter->_fMaxTime;
		if (_fRatio >= 1.f)
		{
			_fRatio = 1.f;
		}

		if (iter->_tTask == TASK_TYPE::FADE_IN_LIGHT_COLOR)
		{
			_lightColor.x = _fRatio * iter->_vColor.x;
			_lightColor.y = _fRatio * iter->_vColor.y;
			_lightColor.z = _fRatio * iter->_vColor.z;

			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetColor(_lightColor);
		}
		else if (iter->_tTask == TASK_TYPE::FADE_OUT_LIGHT_COLOR)
		{
			_lightColor.x = iter->_vColor.x - (_fRatio * iter->_vColor.x);
			_lightColor.y = iter->_vColor.y - (_fRatio * iter->_vColor.y);
			_lightColor.z = iter->_vColor.z - (_fRatio * iter->_vColor.z);
			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetColor(_lightColor);
		}
		else if (iter->_tTask == TASK_TYPE::FADE_IN_LIGHT_RADIUS)
		{
			_radius = iter->_fMaxRadius * _fRatio;
			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetRadius(_radius);
		}
		else
		{
			_radius = iter->_fMaxRadius - (iter->_fMaxRadius * _fRatio);
			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetRadius(_radius);
		}

		if (_fRatio >= 1.f)
		{
			iter = m_LightTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
