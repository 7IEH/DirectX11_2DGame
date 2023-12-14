#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_Transform{}
	, m_Matrix{}
{
	m_Matrix = new transform();
	m_Transform = new tTransform();
	m_Transform->_Position = {};
	m_Transform->_Scale = { 23.f,25.f,1.f,2.f };
	m_Transform->_Rotation = 0.f;
}

Transform::~Transform()
{
}

void Transform::Tick()
{
	// world(SRT)
	Vec4 _Scale = m_Transform->_Scale;
	float _Rotation = m_Transform->_Rotation;
	Vec4 _Position = m_Transform->_Position;
	XMMATRIX _scaleMatrix = XMMatrixTranspose(XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z));
	XMMATRIX _rotateMatrix = XMMatrixTranspose(XMMatrixRotationZ(_Rotation * (3.141592f / 180.f)));
	XMMATRIX _transformMatrix = XMMatrixTranspose(XMMatrixTranslation(_Position.x, _Position.y, _Position.z));
	XMMATRIX _temp = XMMatrixMultiply(_scaleMatrix, _rotateMatrix);
	m_Matrix->_world = XMMatrixMultiply(_temp, _transformMatrix);

	// View(Camera)
	XMVECTOR pos = {0.f,0.f,-200.f,1.0f };
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = { 0.0f,1.0f,0.0f,0.0f };
	
	m_Matrix->_view = XMMatrixTranspose(XMMatrixLookAtLH(pos, target, up));

	// Projection(Projection)
	m_Matrix->_projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(45.f * (3.141592f / 180.f), ASPECT_RATIO, 1.f, 1000.0f));
}