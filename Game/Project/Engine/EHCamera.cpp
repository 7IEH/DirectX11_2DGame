#include "pch.h"
#include "EHCamera.h"

#include "EHDevice.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHLevelMgr.h"
#include "EHRenderMgr.h"

#include "EHLevel.h"

extern transform e_MatrixData;

Camera::Camera()
	:Component(COMPONENT_TYPE::CAMERA)
	, m_Projection(PROJECTION_TYPE::ORTHOGRAPHIC)
	, m_FOV(45.f * (3.141592f / 180.f))
	, m_Width(1600.f)
	, m_Scale(1.f)
	, m_AspectRatio(ASPECT_RATIO)
	, m_Far(10000.0f)
	, m_Type(CAMERA_TYPE::NON_TYPE)
	, m_LayerVisible(0)
{
	Vec2 vResol = Device::GetInst()->GetResolution();
	m_Width = vResol.x;
}

Camera::~Camera()
{
}

/*********************************
|	Camera View Projection Part
**********************************/

void Camera::LateUpdate()
{
	Vec4 _pos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	// W = (RT)^-1 -> T^-1 * R^-1
	Matrix _reverseTransform = XMMatrixTranslation(-_pos.x, -_pos.y, -_pos.z);

	Vec3 _Right = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::RIGHT);
	Vec3 _UP = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::UP);
	Vec3 _Front = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::FRONT);

	Matrix _reverseRotation = { _Right.x,_Right.y,_Right.z,0,
								_UP.x,_UP.y,_UP.z,0,
								_Front.x,_Front.y,_Front.z,0,
								0,0,0,1 };

	/*Matrix _reverseRotation = { _Right.x,_Right.y,_Right.z,0,
								_UP.x,_UP.y,_UP.z,0,
								_Front.x,_Front.y,_Front.z,0,
								0,0,0,1 };*/

	/**************************************
	< -  View Matrix Caculate

					
	Store Class Member variable for Render
	**************************************/

	// 1. View Matrix
	m_ViewMat = XMMatrixMultiply(_reverseTransform, _reverseRotation);

	// 2. Projection Matrix
	switch (m_Projection)
	{
	case PROJECTION_TYPE::PERSPECTIVE:
	{
		ProjectiveView();
	}
	break;
	case PROJECTION_TYPE::ORTHOGRAPHIC:
	{
		OrthographicView();
	}
	break;
	case PROJECTION_TYPE::END:
		break;
	default:
		break;
	}
}

void Camera::InitializeDir()
{
	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->InitializeDir();
}

void Camera::ProjectiveView()
{
	// Projection(Projection)
	m_ProjMat = XMMatrixPerspectiveFovLH(m_FOV, ASPECT_RATIO, 1.f, m_Far);
}

void Camera::OrthographicView()
{
	// Projection(Orthographic)
	m_ProjMat = XMMatrixOrthographicLH(m_Width * m_Scale, 900.f * m_Scale, 1.f, m_Far);
}

/************************
|	Camera Render Part
************************/

void Camera::Render()
{
	e_MatrixData.View = m_ViewMat;
	e_MatrixData.Projection = m_ProjMat;

	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	for (int i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		if (false == (m_LayerVisible & (1 << i)))
			continue;

		Layer* _layer = _curLevel->GetLayer(LAYER_TYPE(i));
		const vector<GameObject*>& _objects = _layer->GetLayerObject();

		for (size_t i = 0;i < _objects.size();i++)
		{
			_objects[i]->Render();
		}
	}
}

void Camera::LayerVisibleSet(LAYER_TYPE _type, bool _visible)
{
	if (_visible)
	{
		m_LayerVisible |= (1 << (UINT)_type);
	}
	else
	{
		m_LayerVisible &= ~(1 << (UINT)_type);
	}
}

void Camera::AllVisibleSet(bool _visible)
{
	if (_visible)
	{
		m_LayerVisible = 0xffffffff;
	}
	else
	{
		m_LayerVisible = 0;
	}
}

void Camera::SetCameraType(CAMERA_TYPE _type)
{
	m_Type = _type;
	RenderMgr::GetInst()->RegisterCamera(_type, GetOwner());
}
