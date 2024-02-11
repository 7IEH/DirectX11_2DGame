#include "pch.h"

#include "EHTaskMgr.h"
#include "EHRenderMgr.h"
#include "EHLevelMgr.h"

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


void Object::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void Object::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot;

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.matWorld = XMMatrixTranspose(info.matWorld);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void Object::DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void Object::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.matWorld = XMMatrixTranspose(info.matWorld);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void SceneManager::LoadScene(const wstring& _sceneName)
{
	Level* _loadLevel = LevelMgr::GetInst()->FindLevel(_sceneName);

	EHTask _task = {};
	_task._Type = TASK_TYPE::LEVEL_CHANGE;
	_task.Param_1 = (UINT_PTR)_loadLevel;
	TaskMgr::GetInst()->AddTask(_task);
}

string EH::ConvertString(wstring& _wstr)
{
	return string(_wstr.begin(), _wstr.end());
}

wstring EH::ConvertWstring(string& _str)
{
	return wstring(_str.begin(), _str.end());
}

string EH::ConvertString(const std::filesystem::path::string_type _str)
{
	return string(_str.begin(), _str.end());
}

void EH::InputVector3(string _line, OUT Vec3& _output)
{
	int _prev = 0;
	int _next = 0;
	string _temp = "";

	for (int j = 0;j < 3;j++)
	{
		_next = _line.find(" ", _prev + 1);
		_temp = _line.substr(_prev, _next - _prev);
		_prev = _next;
		if (j == 0)
		{
			_output.x = stof(_temp);
		}
		else if (j == 1)
		{
			_output.y = stof(_temp);
		}
		else
		{
			_output.z = stof(_temp);
		}
	}
}

void EH::InputVector3(string _line, OUT Vec4& _output)
{
	int _prev = 0;
	int _next = 0;
	string _temp = "";

	for (int j = 0;j < 3;j++)
	{
		_next = _line.find(" ", _prev + 1);
		_temp = _line.substr(_prev, _next - _prev);
		_prev = _next;
		if (j == 0)
		{
			_output.x = stof(_temp);
		}
		else if (j == 1)
		{
			_output.y = stof(_temp);
		}
		else
		{
			_output.z = stof(_temp);
		}
	}
}

void EH::InputVector4(string _line, OUT Vec4& _output)
{
	int _prev = 0;
	int _next = 0;
	string _temp = "";

	for (int j = 0;j < 4;j++)
	{
		_next = _line.find(" ", _prev + 1);
		_temp = _line.substr(_prev, _next - _prev);
		_prev = _next;
		if (j == 0)
		{
			_output.x = stof(_temp);
		}
		else if (j == 1)
		{
			_output.y = stof(_temp);
		}
		else if(j==2)
		{
			_output.z = stof(_temp);
		}
		else
		{
			_output.w = stof(_temp);
		}
	}
}
