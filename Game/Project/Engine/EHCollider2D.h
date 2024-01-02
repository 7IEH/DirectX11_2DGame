#pragma once
#include "EHComponent.h"
class Collider2D :
	public Component
{
private:
	Vec3			m_OffsetPos;
	Vec3			m_OffsetSize;

	Matrix			m_OffsetMat;


	UINT			m_CollisionCount;

	bool			m_Absolute;
	bool			m_Enabled;

	bool			m_IsTrigger;

public:
	virtual void LateUpdate();

public:
	Vec3    GetOffsetPos() { return m_OffsetPos; }
	Vec3    GetOffsetScale() { return m_OffsetSize; }
	Matrix	GetOffsetMatrix() { return m_OffsetMat; }

	void    SetOffsetPos(Vec3 _OffsetPos) { m_OffsetPos = _OffsetPos; }
	void    SetoffSetScale(Vec3 _OffsetSize) { m_OffsetSize = _OffsetSize; }

	void    Enabled(bool _Enabled) { m_Enabled = _Enabled; }
	void    IsTrigger(bool _IsTrigger) { m_IsTrigger = _IsTrigger; }

public:
	Collider2D();
	virtual ~Collider2D();
};

