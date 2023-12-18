#include "pch.h"
#include "EHCamera.h"

#include "EHDevice.h"

#include "EHGameObject.h"
#include "EHTransform.h"

Camera::Camera()
	:Component(COMPONENT_TYPE::CAMERA)
	, m_Projection(PROJECTION_TYPE::PERSPECTIVE)
	, m_FOV(45.f * (3.141592f / 180.f))
	, m_Width(1.f)
	, m_Scale(1.f)
	, m_AspectRatio(ASPECT_RATIO)
	, m_Far(1000.0f)
	, m_matView()
	, m_matProj()
{
}

Camera::~Camera()
{
}

void Camera::Tick()
{
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

void Camera::ProjectiveView()
{
	XMVECTOR pos;
	XMVECTOR target;
	XMVECTOR up;

	if (m_Target != nullptr)
	{
		Vec4 _pos = m_Target->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetTransform()->_Position;
		pos = { _pos.x,_pos.y,-50.f,1.0f };
		target = { _pos.x,_pos.y,_pos.z,_pos.w };
		up = { 0.0f,1.0f,0.0f,0.0f };
	}
	else 
	{
		pos = { 0.f,0.f,-50.f,1.0f };
		target = XMVectorZero();
		up = { 0.0f,1.0f,0.0f,0.0f };
	}

	m_matView = XMMatrixTranspose(XMMatrixLookAtLH(pos, target, up));

	// Projection(Projection)
	m_matProj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_FOV, ASPECT_RATIO, 1.f, m_Far));
}

void Camera::OrthographicView()
{
}


