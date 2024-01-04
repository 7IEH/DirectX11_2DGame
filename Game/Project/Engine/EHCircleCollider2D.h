#pragma once

#include "EHCollider.h"

class CircleCollider2D
	:public Collider
{
private:
	float	m_Radius;
	Vec3	m_OffsetPos;

	Matrix	m_OffsetMat;

public:
	void SetRadius(float _radius) { m_Radius = _radius; }
	float GetRadius() { return m_Radius; }

	void SetOffsetPos(Vec3 _offsetPos) { m_OffsetPos = _offsetPos; }
	Vec3 GetOffsetPos() { return m_OffsetPos; }

	Matrix GetOffsetMat() { return m_OffsetMat; }

public:
	virtual void LateUpdate() override;
	virtual void DrawCollider() override;

public:
	CircleCollider2D();
	virtual ~CircleCollider2D();
};

