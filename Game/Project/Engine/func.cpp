#include "pch.h"

#include "EHTaskMgr.h"
#include "EHRenderMgr.h"
#include "EHLevelMgr.h"

#include "EHAssetMgr.h"
#include "EHSound.h"

extern wstring e_sNxtScene;

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

void Object::FadeIn(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_IN;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeOut(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_OUT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeInLightAmbient(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_IN_LIGHT_AMBIENT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeOutLightAmbient(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_OUT_LIGHT_AMBIENT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeInLightColor(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_IN_LIGHT_COLOR;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeOutLightColor(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_OUT_LIGHT_COLOR;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeInLightRadius(GameObject* _target, float _radius, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_IN_LIGHT_RADIUS;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	_task.Param_3 = (UINT_PTR)_radius;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeOutLightRadius(GameObject* _target, float _radius, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_OUT_LIGHT_RADIUS;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	_task.Param_3 = (UINT_PTR)_radius;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeInText(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_IN_TEXT_COLOR;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::FadeOutText(GameObject* _target, float _time)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::FADE_OUT_TEXT_COLOR;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_time;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::MoveUp(GameObject* _target, float _dest, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::MOVE_UP;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_4 = (INT_PTR)_dest;
	_task.Param_3 = (UINT_PTR)_speed;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::MoveDown(GameObject* _target, float _dest, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::MOVE_DOWN;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_4 = (INT_PTR)_dest;
	_task.Param_3 = (UINT_PTR)_speed;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::MoveRight(GameObject* _target, float _dest, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::MOVE_RIGHT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_4 = (INT_PTR)_dest;
	_task.Param_3 = (UINT_PTR)_speed;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::MoveLeft(GameObject* _target, float _dest, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::MOVE_LEFT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_4 = (INT_PTR)_dest;
	_task.Param_3 = (UINT_PTR)_speed;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::GrowEffect(GameObject* _target, float _scale, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::GROW_EFFECT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_scale;
	_task.Param_3 = (UINT_PTR)_speed;
	TaskMgr::GetInst()->AddTask(_task);
}

void Object::DecreaseEffect(GameObject* _target, float _scale, float _speed)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::DECREASE_EFFECT;
	_task.Param_1 = (UINT_PTR)_target;
	_task.Param_2 = (UINT_PTR)_scale;
	_task.Param_3 = (UINT_PTR)_speed;
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

void Object::ShakingEffect(float _fTime, float _fSpeed, float _fDistance)
{
	EHTask _task = {};
	_task._Type = TASK_TYPE::SHAKE_EFFECT;
	_task.Param_1 = (UINT_PTR)_fTime;
	_task.Param_2 = (UINT_PTR)_fSpeed;
	_task.Param_3 = (UINT_PTR)_fDistance;

	TaskMgr::GetInst()->AddTask(_task);
}

void Object::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
	Ptr<Sound> pSound = AssetMgr::GetInst()->Load<Sound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Play(_Loop, _Volume, _Overlap);
	}
}

void Object::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
	static Ptr<Sound> CurBGM = nullptr;

	Ptr<Sound> pSound = AssetMgr::GetInst()->Load<Sound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		if (nullptr != CurBGM)
		{
			CurBGM->Stop();
		}

		pSound->Play(0, _Volume);
		CurBGM = pSound;
	}
}

void Object::Stop2DSound(const wstring& _SoundPath)
{
	Ptr<Sound> pSound = AssetMgr::GetInst()->Load<Sound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Stop();
	}
}

void SceneManager::LoadScene(const wstring& _sceneName)
{
	Level* _loadLevel = LevelMgr::GetInst()->FindLevel(_sceneName);

	EHTask _task = {};
	_task._Type = TASK_TYPE::LEVEL_CHANGE;
	_task.Param_1 = (UINT_PTR)_loadLevel;
	TaskMgr::GetInst()->AddTask(_task);
}

void SceneManager::SelectScene(const wstring& _sceneName)
{
	Object::FadeOutLightAmbient(FIND_OBJECT(L"MainLight"), 2.5f);
	Object::Play2DSound(L"\\resource\\Audio\\main_menu_door_absorb.wav", TRUE, 0.5f);
	EHTask _task = {};
	_task._Type = TASK_TYPE::LEVEL_CHANGE;
	TaskMgr::GetInst()->AddTask(_task);
	e_sNxtScene = _sceneName;
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

void EH::InputVector2(string _line, OUT Vec2& _output)
{
	int _prev = 0;
	int _next = 0;
	string _temp = "";

	for (int j = 0;j < 2;j++)
	{
		_next = (int)_line.find(" ", _prev + 1);
		_temp = _line.substr(_prev, _next - _prev);
		_prev = _next;
		if (j == 0)
		{
			_output.x = stof(_temp);
		}
		else
		{
			_output.y = stof(_temp);
		}
	}
}

void EH::InputVector3(string _line, OUT Vec3& _output)
{
	int _prev = 0;
	int _next = 0;
	string _temp = "";

	for (int j = 0;j < 3;j++)
	{
		_next = (int)_line.find(" ", _prev + 1);
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
		_next = (int)_line.find(" ", _prev + 1);
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
		_next = (int)_line.find(" ", _prev + 1);
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
		else if (j == 2)
		{
			_output.z = stof(_temp);
		}
		else
		{
			_output.w = stof(_temp);
		}
	}
}

string EH::WriteVector2(Vec2 _data)
{
	string _result = "";

	_result += std::to_string(_data.x);
	_result += " ";
	_result += std::to_string(_data.y);
	return _result;
}

string EH::WriteVector3(Vec3 _data)
{
	string _result = "";

	_result += std::to_string(_data.x);
	_result += " ";
	_result += std::to_string(_data.y);
	_result += " ";
	_result += std::to_string(_data.z);
	return _result;
}

string EH::WriteVector4(Vec4 _data)
{
	string _result = "";
	_result += std::to_string(_data.x);
	_result += " ";
	_result += std::to_string(_data.y);
	_result += " ";
	_result += std::to_string(_data.z);
	_result += " ";
	_result += std::to_string(_data.w);
	return _result;
}

wstring EH::wWriteVector2(Vec2 _data)
{
	wstring _result = L"";

	_result += std::to_wstring(_data.x);
	_result += L" ";
	_result += std::to_wstring(_data.y);
	return _result;
}

wstring EH::wWriteVector3(Vec3 _data)
{
	wstring _result = L"";

	_result += std::to_wstring(_data.x);
	_result += L" ";
	_result += std::to_wstring(_data.y);
	_result += L" ";
	_result += std::to_wstring(_data.z);
	return _result;
}

wstring EH::wWriteVector4(Vec4 _data)
{
	wstring _result = L"";

	_result += std::to_wstring(_data.x);
	_result += L" ";
	_result += std::to_wstring(_data.y);
	_result += L" ";
	_result += std::to_wstring(_data.z);
	_result += L" ";
	_result += std::to_wstring(_data.w);
	return _result;
}

void EH::InputVector2(wstring _line, OUT Vec2& _output)
{
	int _prev = 0;
	int _next = 0;
	wstring _temp = L"";

	for (int j = 0;j < 2;j++)
	{
		_next = (int)_line.find(L" ", _prev + 1);
		_temp = _line.substr(_prev, _next - _prev);
		_prev = _next;
		if (j == 0)
		{
			_output.x = stof(_temp);
		}
		else
		{
			_output.y = stof(_temp);
		}
	}
}

void EH::InputVector3(wstring _line, OUT Vec3& _output)
{
	int _prev = 0;
	int _next = 0;
	wstring _temp = L"";

	for (int j = 0;j < 3;j++)
	{
		_next = (int)_line.find(L" ", _prev + 1);
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

void EH::InputVector3(wstring _line, OUT Vec4& _output)
{
	int _prev = 0;
	int _next = 0;
	wstring _temp = L"";

	for (int j = 0;j < 3;j++)
	{
		_next = (int)_line.find(L" ", _prev + 1);
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

void EH::InputVector4(wstring _line, OUT Vec4& _output)
{
	int _prev = 0;
	int _next = 0;
	wstring _temp = L"";

	for (int j = 0;j < 3;j++)
	{
		_next = (int)_line.find(L" ", _prev + 1);
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
		else if (j == 2)
		{
			_output.z = stof(_temp);
		}
		else
		{
			_output.w = stof(_temp);
		}
	}
}
