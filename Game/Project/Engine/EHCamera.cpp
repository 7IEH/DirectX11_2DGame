#include "pch.h"
#include "EHCamera.h"

#include "EHDevice.h"

#include "EHGameObject.h"
#include "EHTransform.h"

extern transform e_MatrixData;

Camera::Camera()
	:Component(COMPONENT_TYPE::CAMERA)
	, m_Projection(PROJECTION_TYPE::ORTHOGRAPHIC)
	, m_FOV(45.f * (3.141592f / 180.f))
	, m_Width(1600.f)
	, m_Scale(1.f)
	, m_AspectRatio(ASPECT_RATIO)
	, m_Far(10000.0f)
{
	Vec2 vResol = Device::GetInst()->GetResolution();
	m_Width = vResol.x;
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
	Vec4 _pos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	// W = (RT)^-1 -> T^-1 * R^-1
	Matrix _reverseTransform = XMMatrixTranspose(XMMatrixTranslation(-_pos.x, -_pos.y, -_pos.z));

	Vec3 _Right = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::RIGHT);
	Vec3 _UP = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::UP);
	Vec3 _Front = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetWorldDir(DIRECTION_TYPE::FRONT);

	Matrix _reverseRotation = { _Right.x,_Right.y,_Right.z,0,
								_UP.x,_UP.y,_UP.z,0,
								_Front.x,_Front.y,_Front.z,0,
								0,0,0,1 };

	// View Matrix
	e_MatrixData.View = XMMatrixMultiply(_reverseTransform, _reverseRotation);
	

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
	e_MatrixData.Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_FOV, ASPECT_RATIO, 1.f, m_Far));
}

void Camera::OrthographicView()
{
	// Projection(Orthographic)
	e_MatrixData.Projection = XMMatrixTranspose(XMMatrixOrthographicLH(m_Width * m_Scale, 900.f * m_Scale, 1.f, m_Far));
}


