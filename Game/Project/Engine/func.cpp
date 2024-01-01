#include "pch.h"

#include "EHTaskMgr.h"
#include "EHRenderMgr.h"

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
