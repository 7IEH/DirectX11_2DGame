#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"
#include "EHLevelMgr.h"

#include"EHGameObject.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeTransform{}
	, m_RelativeWorld{}
	, m_LocalDir{}
	, m_WorldDir{}
	, m_Absolute(true)
{
	m_RelativeTransform = new tTransform();
}

Transform::~Transform()
{
}

void Transform::FinalTick()
{
	// world(SRT)
	Vec4 _Scale = m_RelativeTransform->_Scale;
	Vec3 _Rotation = m_RelativeTransform->_Rotation;
	Vec4 _Position = m_RelativeTransform->_Position;
	
	XMMATRIX _scaleMatrix = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	XMMATRIX _rotateMatrixX = XMMatrixRotationX(_Rotation.x * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixY = XMMatrixRotationY(_Rotation.y * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixZ = XMMatrixRotationZ(_Rotation.z * (XM_PI / 180.f));
	XMMATRIX _transformMatrix = XMMatrixTranslation(_Position.x, _Position.y, _Position.z);

	XMMATRIX _temp = XMMatrixMultiply(_scaleMatrix, _rotateMatrixX);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixY);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixZ);
	_temp = XMMatrixMultiply(_temp, _transformMatrix);
	m_RelativeWorld = XMMatrixTranspose(_temp);

	// 물체의 방향값을 다시 계산한다.
	m_WorldDir[(UINT)DIRECTION_TYPE::RIGHT] = m_LocalDir[(UINT)DIRECTION_TYPE::RIGHT] = { 1.f,0.f,0.f,0.f };
	m_WorldDir[(UINT)DIRECTION_TYPE::UP] = m_LocalDir[(UINT)DIRECTION_TYPE::UP] = { 0.f,1.f,0.f,0.f };
	m_WorldDir[(UINT)DIRECTION_TYPE::FRONT] = m_LocalDir[(UINT)DIRECTION_TYPE::FRONT] = { 0.f,0.f,1.f,0.f };
	
	// Vec3 를 vec4 타입으로 확장
	//XMVector3TransformCoord w를 1로 확장
	//XMVector3TransformNormal w를 0으로 확장

	for (int i = 0;i < (UINT)DIRECTION_TYPE::END;i++)
	{
		m_LocalDir[i] = XMVector3TransformNormal(m_LocalDir[i], m_RelativeWorld);
		// Normalize;
		m_LocalDir[i].Normalize();
	}

	GameObject* _parent = GetOwner()->GetParent();
	if (_parent != nullptr)
	{
		Transform* _parenttr = _parent->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		XMMATRIX _parentMat = _parenttr->GetMatWorld();
		_parentMat = XMMatrixTranspose(_parentMat);
		if (m_Absolute)
		{
			Vec4 vParentScale = _parenttr->GetRelativeScale();

			XMMATRIX matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_RelativeWorld = _temp * matParentScaleInv * _parentMat;
			m_RelativeWorld = XMMatrixTranspose(m_RelativeWorld);
		}
		else
		{
			m_RelativeWorld = XMMatrixMultiply(_temp, _parentMat);
			m_RelativeWorld = XMMatrixTranspose(m_RelativeWorld);
		}
	}

	for (int i = 0;i < (UINT)DIRECTION_TYPE::END;i++)
	{
		m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_RelativeWorld);
		// NOrmalize;
		m_WorldDir[i].Normalize();
	}
}

void Transform::InitializeDir()
{
	m_RelativeTransform->_Rotation = { 0.f,0.f,0.f };
}