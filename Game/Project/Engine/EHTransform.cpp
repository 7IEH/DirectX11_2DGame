#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"
#include "EHLevelMgr.h"

#include "EHDevice.h"

#include "EHConstantBuffer.h"
#include "EHGameObject.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeTransform{}
	, m_RelativeWorld{}
	, m_LocalDir{}
	, m_WorldDir{}
	, m_Absolute(true)
{
	m_RelativeTransform._Scale = { 1.f,1.f,1.f,1.f };
}

Transform::~Transform()
{
}

void Transform::LateUpdate()
{
	// world(SRT)
	Vec4 _Scale = m_RelativeTransform._Scale;
	Vec3 _Rotation = m_RelativeTransform._Rotation;
	Vec4 _Position = m_RelativeTransform._Position;

	XMMATRIX _scaleMatrix = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	XMMATRIX _rotateMatrixX = XMMatrixRotationX(_Rotation.x * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixY = XMMatrixRotationY(_Rotation.y * (XM_PI / 180.f));
	XMMATRIX _rotateMatrixZ = XMMatrixRotationZ(_Rotation.z * (XM_PI / 180.f));
	XMMATRIX _transformMatrix = XMMatrixTranslation(_Position.x, _Position.y, _Position.z);

	XMMATRIX _temp = XMMatrixMultiply(_scaleMatrix, _rotateMatrixX);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixY);
	_temp = XMMatrixMultiply(_temp, _rotateMatrixZ);
	_temp = XMMatrixMultiply(_temp, _transformMatrix);
	m_RelativeWorld = _temp;
	
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
		if (m_Absolute)
		{
			Vec4 vParentScale = _parenttr->GetRelativeScale();

			XMMATRIX matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_RelativeWorld = _temp * matParentScaleInv * _parentMat;
		}
		else
		{
			m_RelativeWorld = XMMatrixMultiply(_temp, _parentMat);
		}
	}

	for (int i = 0;i < (UINT)DIRECTION_TYPE::END;i++)
	{
		m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_RelativeWorld);
		// NOrmalize;
		m_WorldDir[i].Normalize();
	}
}

void Transform::UpdateData()
{
	e_MatrixData.World = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();

	e_MatrixData.WorldInv = XMMatrixInverse(nullptr, e_MatrixData.World);
	
	e_MatrixData.WorldInv = XMMatrixTranspose(e_MatrixData.WorldInv);

	e_MatrixData.ViewInv = XMMatrixInverse(nullptr, e_MatrixData.View);

	e_MatrixData.ViewInv = XMMatrixTranspose(e_MatrixData.ViewInv);

	e_MatrixData.ProjInv = XMMatrixInverse(nullptr, e_MatrixData.Projection);

	e_MatrixData.ProjInv = XMMatrixTranspose(e_MatrixData.ProjInv);

	XMMATRIX _worldtemp = e_MatrixData.World;
	XMMATRIX _viewtemp = e_MatrixData.View;
	XMMATRIX _projectiontemp = e_MatrixData.Projection;

	e_MatrixData.WV = XMMatrixMultiply(_worldtemp, _viewtemp);

	// WVP
	e_MatrixData.WVP = XMMatrixTranspose(XMMatrixMultiply(e_MatrixData.WV, _projectiontemp));

	// WV 
	e_MatrixData.WV = XMMatrixTranspose(e_MatrixData.WV);

	// World
	e_MatrixData.World = XMMatrixTranspose(e_MatrixData.World);


	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->SetData(&e_MatrixData, sizeof(transform), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData();
}

Vec3 Transform::GetWorldScale()
{
	GameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = Vec3(m_RelativeTransform._Scale);

	while(pParent)
	{
		vWorldScale *= Vec3(pParent->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale());
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void Transform::InitializeDir()
{
	m_RelativeTransform._Rotation = { 0.f,0.f,0.f };
}