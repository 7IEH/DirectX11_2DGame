#include "pch.h"
#include "EHCollider2D.h"

#include "EHGameObject.h"

Collider2D::Collider2D()
	:Component(COMPONENT_TYPE::COLLIDER2D)
	, m_OffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_OffsetSize(Vec3(1.f, 1.f, 1.f))
	, m_OffsetMat{}
	, m_CollisionCount(0)
	, m_Absolute(FALSE)
	, m_Enabled(TRUE)
	, m_IsTrigger(FALSE)
{
}

Collider2D::~Collider2D()
{

}

void Collider2D::LateUpdate()
{
	/************************
	| Collider Matrix Update
	************************/
	Matrix _Scale = XMMatrixScaling(m_OffsetSize.x, m_OffsetSize.y, m_OffsetSize.z);
	Matrix _Pos = XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);

	m_OffsetMat = XMMatrixMultiply(_Scale, _Pos);

	/************************
	| Base Matrix Update
	************************/
	Matrix _OwnerWorld = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();
	Vec4  _OwnerScale = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();

	Matrix _OwnerScaleMat = XMMatrixScaling(_OwnerScale.x, _OwnerScale.y, _OwnerScale.z);
	_OwnerScaleMat = XMMatrixInverse(nullptr, _OwnerScaleMat);

	if (m_Absolute)
	{
		// Scale Normalized
		_OwnerWorld = XMMatrixMultiply(_OwnerScaleMat, _OwnerWorld);
	}

	m_OffsetMat = XMMatrixMultiply(m_OffsetMat, _OwnerWorld);

	//Draw
	if (m_CollisionCount == 0)
	{
		Object::DrawDebugRect(m_OffsetMat, Vec3(0.f, 1.f, 0.f), TRUE, 0.f);
	}
	else
	{
		Object::DrawDebugRect(m_OffsetMat, Vec3(1.f, 0.f, 0.f), TRUE, 0.f);
	}
}

void Collider2D::OnTriggerEnter(Collider2D* _other)
{
	m_CollisionCount++;
}

void Collider2D::OnTriggerStay(Collider2D* _other)
{
}

void Collider2D::OnTriggerExit(Collider2D* _other)
{
	m_CollisionCount--;
}