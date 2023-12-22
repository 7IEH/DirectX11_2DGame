#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"
#include "EHLevelMgr.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_Transform{}
	, m_matWorld{}
	, m_Dir{}
{
	m_Transform = new tTransform();
	m_Transform->_Scale = { 1.f,1.f,1.f,1.f };
	m_Transform->_Rotation = { 0.f,0.f,0.f };
}

Transform::~Transform()
{
}

void Transform::FinalTick()
{
	// world(SRT)
	Vec4 _Scale = m_Transform->_Scale;
	Vec3 _Rotation = m_Transform->_Rotation;
	Vec4 _Position = m_Transform->_Position;
	
	XMMATRIX _scaleMatrix = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	XMMATRIX _rotateMatrixX = XMMatrixRotationX(_Rotation.x * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixY = XMMatrixRotationY(_Rotation.y * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixZ = XMMatrixRotationZ(_Rotation.z * (XM_PI / 180.f));
	XMMATRIX _transformMatrix = XMMatrixTranslation(_Position.x, _Position.y, _Position.z);

	XMMATRIX _temp = XMMatrixMultiply(_scaleMatrix, _rotateMatrixX);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixY);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixZ);
	m_matWorld = XMMatrixTranspose(XMMatrixMultiply(_temp, _transformMatrix));
	
	// ��ü�� ���Ⱚ�� �ٽ� ����Ѵ�.
	m_Dir[(UINT)DIRECTION_TYPE::RIGHT] = { 1.f,0.f,0.f,0.f };
	m_Dir[(UINT)DIRECTION_TYPE::UP] = { 0.f,1.f,0.f,0.f };
	m_Dir[(UINT)DIRECTION_TYPE::FRONT] = { 0.f,0.f,1.f,0.f };


	// Vec3 �� vec4 Ÿ������ Ȯ��
	//XMVector3TransformCoord w�� 1�� Ȯ��
	//XMVector3TransformNormal w�� 0���� Ȯ��

	for (int i = 0;i < (UINT)DIRECTION_TYPE::END;i++)
	{
		m_Dir[i] = XMVector3TransformNormal(m_Dir[i], m_matWorld);
		// NOrmalize;
		m_Dir[i].Normalize();
	}
}

void Transform::InitializeDir()
{
	m_Transform->_Rotation = { 0.f,0.f,0.f };
}