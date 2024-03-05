#include "pch.h"
#include "EHCircleCollider2D.h"

#include "EHGameObject.h"

CircleCollider2D::CircleCollider2D()
	:Collider(COLLIDER_TYPE::CIRCLECOLLDIER2D)
	, m_Radius(0.f)
	, m_OffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_OffsetMat{}
{
}

CircleCollider2D::CircleCollider2D(const CircleCollider2D& _origin)
	:Collider(_origin)
	, m_Radius(_origin.m_Radius)
	, m_OffsetPos(_origin.m_OffsetPos)
	, m_OffsetMat(_origin.m_OffsetMat)
{
}

CircleCollider2D::~CircleCollider2D()
{
}

void CircleCollider2D::LateUpdate()
{
	/************************
	| Collider Matrix Update
	************************/
	Matrix _Scale = XMMatrixScaling(m_Radius, m_Radius, 1.f);
	Matrix _Pos = XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);

	m_OffsetMat = XMMatrixMultiply(_Scale, _Pos);

	/************************
	| Base Matrix Update
	************************/
	Matrix _OwnerWorld = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();
	Vec4  _OwnerScale = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();

	Matrix _OwnerScaleMat = XMMatrixScaling(_OwnerScale.x, _OwnerScale.y, _OwnerScale.z);
	_OwnerScaleMat = XMMatrixInverse(nullptr, _OwnerScaleMat);

	// Scale Normalized
	_OwnerWorld = XMMatrixMultiply(_OwnerScaleMat, _OwnerWorld);

	m_OffsetMat = XMMatrixMultiply(m_OffsetMat, _OwnerWorld);

	//Draw
	DrawCollider();
}

void CircleCollider2D::DrawCollider()
{
	if (GetCollisionCount())
	{
		Object::DrawDebugCircle(m_OffsetMat, Vec3(1.f, 0.f, 0.f), TRUE);
	}
	else
	{
		Object::DrawDebugCircle(m_OffsetMat, Vec3(0.f, 1.f, 0.f), TRUE);
	}
}

void CircleCollider2D::Save(std::wofstream* _file)
{
	// offset scale, offset radius
	Vec3 _offPos = m_OffsetPos;
	float _radius = m_Radius;

	*_file << L"CIRCLECOLLIDER2D\n";

	*_file << std::to_wstring(_radius) + L"\n";
	*_file << EH::wWriteVector3(_offPos) + L"\n";
}

void CircleCollider2D::Load(std::wifstream* _file)
{
	wstring _line = L"";
	for (int i = 0;i < 2;i++)
	{
		std::getline(*_file, _line);
		if (i == 0)
		{
			m_Radius = std::stof(_line);
		}
		else
		{
			EH::InputVector3(_line, m_OffsetPos);
		}
	}
}
