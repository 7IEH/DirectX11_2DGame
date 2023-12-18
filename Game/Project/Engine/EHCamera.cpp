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

void Camera::FinalTick()
{
	XMVECTOR pos;
	XMVECTOR target;
	XMVECTOR up;

	Transform* tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	pos = Vec4(tr->GetTransform()->_Position.x, tr->GetTransform()->_Position.y, -21.0f, 1.f);
	target = tr->GetTransform()->_Position;
	up = Vec4(0.f, 1.f, 0.f, 0.f);

	// View Matrix
	e_MatrixData._view = XMMatrixTranspose(XMMatrixLookAtLH(pos, target, up));

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
	// Projection(Projection)
	e_MatrixData._projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_FOV, ASPECT_RATIO, 1.f, m_Far));
}

void Camera::OrthographicView()
{
	e_MatrixData._projection = XMMatrixTranspose(XMMatrixOrthographicLH(16, 9, 1.f, 1000.f));
	int a = 0;
}


