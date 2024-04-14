#include "pch.h"
#include "EHTaskMgr.h"

#include "EHLevelMgr.h"

#include "EHGameObject.h"
#include "EHLevel.h"
#include "EHComponent.h"

#include "EHTimeMgr.h"
#include "EHText.h"

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
			SceneChange _task = {};
			_task._fAcctime = 0.f;
			m_SceneTasks.push_back(_task);
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
		case TASK_TYPE::FADE_IN_LIGHT_AMBIENT:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_IN_LIGHT_AMBIENT;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._vColor = _effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetAmbient();

			_effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetColor(Vec4(0.f, 0.f, 0.f, 1.f));
			m_AmbientTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_OUT_LIGHT_AMBIENT:
		{
			Light_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_OUT_LIGHT_AMBIENT;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			_effect._vColor = _effect._pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetAmbient();
			m_AmbientTasks.push_back(_effect);
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
		case TASK_TYPE::FADE_IN_TEXT_COLOR:
		{
			Fade_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_IN_TEXT_COLOR;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			m_FontTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::FADE_OUT_TEXT_COLOR:
		{
			Fade_Effect _effect = {};
			_effect._tTask = TASK_TYPE::FADE_OUT_TEXT_COLOR;
			_effect._fAcctime = 0.f;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fMaxTime = (float)m_Tasks[i].Param_2;
			m_FontTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::MOVE_UP:
		{
			Move_Effect _effect = {};
			_effect._tTask = TASK_TYPE::MOVE_UP;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fDest = (float)m_Tasks[i].Param_4;
			_effect._fSpeed = (float)m_Tasks[i].Param_3;
			m_MoveTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::MOVE_DOWN:
		{
			Move_Effect _effect = {};
			_effect._tTask = TASK_TYPE::MOVE_DOWN;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fDest = (float)m_Tasks[i].Param_4;
			_effect._fSpeed = (float)m_Tasks[i].Param_3;
			m_MoveTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::MOVE_RIGHT:
		{
			Move_Effect _effect = {};
			_effect._tTask = TASK_TYPE::MOVE_RIGHT;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fDest = (float)m_Tasks[i].Param_4;
			_effect._fSpeed = (float)m_Tasks[i].Param_3;
			m_MoveTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::MOVE_LEFT:
		{
			Move_Effect _effect = {};
			_effect._tTask = TASK_TYPE::MOVE_LEFT;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			_effect._fDest = (float)m_Tasks[i].Param_4;
			_effect._fSpeed = (float)m_Tasks[i].Param_3;
			m_MoveTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::SHAKE_EFFECT:
		{
			Shake_Effect _effect = {};
			_effect._pObject = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera");
			_effect._vOriginPos = _effect._pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			_effect._fTime = (float)m_Tasks[i].Param_1;
			_effect._fSpeed = (float)m_Tasks[i].Param_2;
			_effect._fDitance = (float)m_Tasks[i].Param_3;
			m_ShakeTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::GROW_EFFECT:
		{
			Scale_Effect _effect = {};
			_effect._Type = TASK_TYPE::GROW_EFFECT;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			Vec4 _vScale = _effect._pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();
			_vScale.x = (float)m_Tasks[i].Param_2;
			_vScale.y = (float)m_Tasks[i].Param_2;
			_effect._vScale = _vScale;
			_effect._fSpeed = (float)m_Tasks[i].Param_2;

			m_ScaleTasks.push_back(_effect);
		}
		break;
		case TASK_TYPE::DECREASE_EFFECT:
		{
			Scale_Effect _effect = {};
			_effect._Type = TASK_TYPE::DECREASE_EFFECT;
			_effect._pObject = (GameObject*)m_Tasks[i].Param_1;
			Vec4 _vScale = _effect._pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();
			_vScale.x = (float)m_Tasks[i].Param_2;
			_vScale.y = (float)m_Tasks[i].Param_2;
			_effect._vScale = _vScale;
			_effect._fSpeed = (float)m_Tasks[i].Param_2;

			m_ScaleTasks.push_back(_effect);
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
	FontUpdate();
	MoveUpdate();
	ShakeUpdate();
	ScaleUpdate();
	AmbientUpdate();
	SceneChangeUpdate();
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

void TaskMgr::AmbientUpdate()
{
	vector<Light_Effect>::iterator iter = m_AmbientTasks.begin();
	for (;iter != m_AmbientTasks.end();)
	{
		Vec4 _ambientColor = Vec4(0.f, 0.f, 0.f, 1.f);
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

		if (iter->_tTask == TASK_TYPE::FADE_IN_LIGHT_AMBIENT)
		{
			_ambientColor.x = _fRatio * iter->_vColor.x;
			_ambientColor.y = _fRatio * iter->_vColor.y;
			_ambientColor.z = _fRatio * iter->_vColor.z;

			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(_ambientColor);
		}
		else if (iter->_tTask == TASK_TYPE::FADE_OUT_LIGHT_AMBIENT)
		{
			_ambientColor.x = iter->_vColor.x - (_fRatio * iter->_vColor.x);
			_ambientColor.y = iter->_vColor.y - (_fRatio * iter->_vColor.y);
			_ambientColor.z = iter->_vColor.z - (_fRatio * iter->_vColor.z);
			iter->_pObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(_ambientColor);
		}

		if (_fRatio >= 1.f)
		{
			iter = m_AmbientTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskMgr::FontUpdate()
{
	vector<Fade_Effect>::iterator iter = m_FontTasks.begin();
	for (;iter != m_FontTasks.end();)
	{
		Text* _text = iter->_pObject->GetComponent<Text>(COMPONENT_TYPE::TEXT);

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

		if (iter->_tTask == TASK_TYPE::FADE_IN_TEXT_COLOR)
		{
			_fAlpha = 0.f;
			_fAlpha = _fRatio;
		}
		else
		{
			_fAlpha = 1.f;
			_fAlpha = (1.f - _fRatio);
		}

		Vec4 _color = _text->GetColor();
		_text->SetColor(Vec4(_color.x, _color.y, _color.z, _fAlpha * 255.f));

		if (_fRatio >= 1.f)
		{
			iter = m_FontTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskMgr::MoveUpdate()
{
	vector<Move_Effect>::iterator iter = m_MoveTasks.begin();
	for (;iter != m_MoveTasks.end();)
	{
		Transform* _pTr = iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vPos = _pTr->GetRelativePosition();

		if (TASK_TYPE::MOVE_UP == iter->_tTask)
		{
			_vPos.y += iter->_fSpeed * DT;

			if (_vPos.y >= iter->_fDest)
			{
				_vPos.y = iter->_fDest;
				iter = m_MoveTasks.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else if (TASK_TYPE::MOVE_DOWN == iter->_tTask)
		{
			float _distance = iter->_fSpeed * DT;
			_vPos.y -= iter->_fSpeed * DT;

			if (_vPos.y <= iter->_fDest)
			{
				_vPos.y = iter->_fDest;
				iter = m_MoveTasks.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else if (TASK_TYPE::MOVE_LEFT == iter->_tTask)
		{
			_vPos.x -= iter->_fSpeed * DT;

			if (_vPos.x <= iter->_fDest)
			{
				_vPos.x = iter->_fDest;
				iter = m_MoveTasks.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else if (TASK_TYPE::MOVE_RIGHT == iter->_tTask)
		{
			_vPos.x += iter->_fSpeed * DT;

			if (_vPos.x >= iter->_fDest)
			{
				_vPos.x = iter->_fDest;
				iter = m_MoveTasks.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		_pTr->SetRelativePosition(_vPos);
	}
}

void TaskMgr::ShakeUpdate()
{
	vector<Shake_Effect>::iterator iter = m_ShakeTasks.begin();

	for (;iter != m_ShakeTasks.end();)
	{
		iter->_fAcctime += DT;

		Vec4 _vPos = iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

		_vPos.x += cosf(iter->_fAcctime * iter->_fSpeed) * iter->_fDitance;
		_vPos.y -= sinf(iter->_fAcctime * iter->_fSpeed) * iter->_fDitance;

		iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);

		if (iter->_fTime <= iter->_fAcctime)
		{
			iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)
				->SetRelativePosition(iter->_vOriginPos);

			iter = m_ShakeTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskMgr::ScaleUpdate()
{
	vector<Scale_Effect>::iterator iter = m_ScaleTasks.begin();

	for (;iter != m_ScaleTasks.end();)
	{
		if (TASK_TYPE::GROW_EFFECT == iter->_Type)
		{
			Transform* _pTr = iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pTr->GetRelativeScale();

			_vScale.x += DT * iter->_fSpeed;
			_vScale.y += DT * iter->_fSpeed;

			if (iter->_vScale.x <= _vScale.x)
			{
				_vScale.x = iter->_vScale.x;
				_vScale.y = iter->_vScale.y;

				iter = m_ScaleTasks.erase(iter);
			}
			else
			{
				iter++;
			}
			_pTr->SetRelativeScale(_vScale);
		}
		else
		{
			Transform* _pTr = iter->_pObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pTr->GetRelativeScale();

			_vScale.x -= DT * iter->_fSpeed;
			_vScale.y -= DT * iter->_fSpeed;

			if (iter->_vScale.x >= _vScale.x)
			{
				_vScale.x = iter->_vScale.x;
				_vScale.y = iter->_vScale.y;

				iter = m_ScaleTasks.erase(iter);
			}
			else
			{
				iter++;
			}
			_pTr->SetRelativeScale(_vScale);
		}
	}
}

void TaskMgr::SceneChangeUpdate()
{
	vector<SceneChange>::iterator iter = m_SceneTasks.begin();
	for (;iter != m_SceneTasks.end();)
	{
		(*iter)._fAcctime += DT;

		if ((*iter)._fAcctime >= 2.5f)
		{
			LevelMgr::GetInst()->SelectLevel(L"LoadingScene");
			iter = m_SceneTasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}