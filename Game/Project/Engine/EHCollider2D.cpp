#include "pch.h"
#include "EHCollider2D.h"

#include "EHGameObject.h"

Collider2D::Collider2D()
	:Collider(COLLIDER_TYPE::BOXCOLLIDER2D)
	, m_OffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_OffsetSize(Vec3(1.f, 1.f, 1.f))
	, m_OffsetMat{}
{
}

Collider2D::Collider2D(const Collider2D& _origin)
	:Collider(_origin)
	, m_OffsetPos(_origin.m_OffsetPos)
	, m_OffsetSize(_origin.m_OffsetSize)
	, m_OffsetMat(_origin.m_OffsetMat)
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

	if (GetAbsolute())
	{
		// Scale Normalized
		_OwnerWorld = XMMatrixMultiply(_OwnerScaleMat, _OwnerWorld);
	}

	m_OffsetMat = XMMatrixMultiply(m_OffsetMat, _OwnerWorld);

	//Draw
	DrawCollider();
}

void Collider2D::DrawCollider()
{
	if (GetCollisionCount() == 0)
	{
		Object::DrawDebugRect(m_OffsetMat, Vec3(0.f, 1.f, 0.f), TRUE, 0.f);
	}
	else
	{
		Object::DrawDebugRect(m_OffsetMat, Vec3(1.f, 0.f, 0.f), TRUE, 0.f);
	}
}

void Collider2D::Save(string _path)
{
	// offset scale, offset position
	std::ofstream _file(_path.data(), std::fstream::out | std::fstream::app);

	Vec3 _offPos = m_OffsetPos;
	Vec3 _offScale = m_OffsetSize;

	_file << "BOXCOLLIDER2D\n";

	_file << EH::WriteVector3(_offScale) + "\n";
	_file << EH::WriteVector3(_offPos) + "\n";

	_file.close();
}

void Collider2D::Load(std::wifstream* _file)
{
	wstring _line = L"";
	for (int i = 0;i < 2;i++)
	{
		std::getline(*_file, _line);
		if (i == 0)
		{
			EH::InputVector3(_line, m_OffsetSize);
		}
		else
		{
			EH::InputVector3(_line, m_OffsetPos);
		}
	}
}
