#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"
#include "EHLevelMgr.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_Transform{}
	, m_matWorld{}
{
	m_Transform = new tTransform();
	m_Transform->_Scale = { 1.f,1.f,1.f,1.f };
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
	m_matWorld = XMMatrixMultiply(_temp, _transformMatrix);
}