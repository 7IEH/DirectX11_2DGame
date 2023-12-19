#include "pch.h"
#include "EHCameraScript.h"

#include "EHGameObject.h"

#include "EHTransform.h"
#include "EHCamera.h"

#include "EHKeyMgr.h"
#include "EHTimeMgr.h"

CameraScript::CameraScript()
	:m_Target(nullptr)
	, m_CamSpeed(100.f)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::Tick()
{
	Camera* _camera = GetOwner()->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
	PROJECTION_TYPE _type = _camera->GetProjectionType();

	if (KEY_TAP(P))
	{
		if (_type == PROJECTION_TYPE::PERSPECTIVE)
		{
			_camera->InitializeDir();
			_camera->SetPorjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		}
		else
		{
			_camera->SetPorjectionType(PROJECTION_TYPE::PERSPECTIVE);
		}
	}

	switch (_type)
	{
	case PROJECTION_TYPE::PERSPECTIVE:
	{
		PerpectiveMove();
	}
	break;
	case PROJECTION_TYPE::ORTHOGRAPHIC:
	{
		OrthoGraphicMove();
	}
	break;
	case PROJECTION_TYPE::END:
		break;
	default:
		break;
	}

	// Target Camera
	/*if (m_Target != nullptr)
		GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)
		->SetPosition(m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetTransform()->_Position);*/
}


void CameraScript::PerpectiveMove()
{
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _pos = _tr->GetPosition();
	Vec3 _rotation = _tr->GetRotation();

	Vec3 _Right = _tr->GetDir(DIRECTION_TYPE::RIGHT);
	Vec3 _UP = _tr->GetDir(DIRECTION_TYPE::UP);
	Vec3 _Front = _tr->GetDir(DIRECTION_TYPE::FRONT);

	if (KEY_PRESSED(A))
	{
		_pos.x -= DT * m_CamSpeed;
	}

	if (KEY_PRESSED(D))
	{
		_pos.x += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(W))
	{
		_pos.z += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(S))
	{
		_pos.z -= DT * m_CamSpeed;
	}

	if (KEY_PRESSED(RBTN))
	{
		_rotation.x += XM_PI * KeyMgr::GetInst()->GetMouseDir().y * DT * m_CamSpeed;
		_rotation.y -= XM_PI * KeyMgr::GetInst()->GetMouseDir().x * DT * m_CamSpeed;
	}

	if (KEY_PRESSED(MBTN))
	{
		_pos.x -= KeyMgr::GetInst()->GetMouseDir().x * DT * m_CamSpeed;
		_pos.y += KeyMgr::GetInst()->GetMouseDir().y * DT * m_CamSpeed;
	}

	_tr->SetDir(DIRECTION_TYPE::RIGHT, _Right);
	_tr->SetDir(DIRECTION_TYPE::UP, _UP);
	_tr->SetDir(DIRECTION_TYPE::FRONT, _Front);
	_tr->SetRotation(_rotation);
	_tr->SetPosition(_pos);
}

void CameraScript::OrthoGraphicMove()
{
	Vec4 _pos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetPosition();
	if (KEY_PRESSED(A))
	{
		_pos.x -= DT * m_CamSpeed;
	}

	if (KEY_PRESSED(D))
	{
		_pos.x += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(W))
	{
		_pos.y += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(S))
	{
		_pos.y -= DT * m_CamSpeed;
	}
	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetPosition(_pos);
}