#pragma once
#include "EHCollider.h"

class Collider2D :
	public Collider
{
private:
	Vec3					m_OffsetPos;
	Vec3					m_OffsetSize;

	Matrix					m_OffsetMat;

public:
	virtual void LateUpdate() override;
	virtual void DrawCollider() override;

	virtual void Save(std::wofstream* _file) override;
	virtual void Load(std::wifstream* _file) override;
public:
	Vec3    GetOffsetPos() { return m_OffsetPos; }
	Vec3    GetOffsetScale() { return m_OffsetSize; }
	Matrix	GetOffsetMatrix() { return m_OffsetMat; }

	void    SetOffsetPos(Vec3 _OffsetPos) { m_OffsetPos = _OffsetPos; }
	void    SetoffSetScale(Vec3 _OffsetSize) { m_OffsetSize = _OffsetSize; }

	CLONE(Collider2D);

public:
	Collider2D();
	Collider2D(const Collider2D& _origin);
	virtual ~Collider2D();


	friend class CollisionMgr;
};

