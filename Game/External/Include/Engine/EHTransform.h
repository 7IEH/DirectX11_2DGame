#pragma once

#include "EHComponent.h"

class Transform
	: public Component
{
private:
	tTransform		m_RelativeTransform;

	XMMATRIX		m_RelativeWorld;

	bool			m_Absolute;

	Vec3			m_LocalDir[(UINT)DIRECTION_TYPE::END];
	Vec3			m_WorldDir[(UINT)DIRECTION_TYPE::END];

public:
	tTransform GetRelativeTransform() { return m_RelativeTransform; }
	XMMATRIX GetMatWorld() { return m_RelativeWorld; }

	void SetRelativeScale(Vec4 _scale) { m_RelativeTransform._Scale = _scale; }
	void SetRelativeRotation(Vec3 _rotation){ m_RelativeTransform._Rotation = _rotation; }
	void SetRelativePosition(Vec4 _position){ m_RelativeTransform._Position = _position; }

	Vec4 GetRelativeScale() { return m_RelativeTransform._Scale; }
	Vec3 GetRelativeRotation() { return m_RelativeTransform._Rotation; }
	Vec4 GetRelativePosition() { return  m_RelativeTransform._Position; }

	Vec3 GetLocalDir(DIRECTION_TYPE _type) { return m_LocalDir[(UINT)_type]; }
	void SetLocalDir(DIRECTION_TYPE _type, Vec3 _dir) { m_LocalDir[(UINT)_type] = _dir; }

	Vec3 GetWorldDir(DIRECTION_TYPE _type) { return m_WorldDir[(UINT)_type]; }
	void SetWorldDir(DIRECTION_TYPE _type, Vec3 _dir) { m_WorldDir[(UINT)_type] = _dir; }

	void InitializeDir();

	void SetAbsolute(bool _Absolute) { m_Absolute = _Absolute; }

public:
	virtual void LateUpdate() override;
	virtual void UpdateData() override;

public:
	Transform();
	virtual ~Transform();
};

