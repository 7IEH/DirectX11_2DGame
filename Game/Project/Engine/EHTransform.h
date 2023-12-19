#pragma once

#include "EHComponent.h"

class Transform
	: public Component
{
private:
	tTransform*		m_Transform;
	XMMATRIX		m_matWorld;

	Vec3			m_Dir[(UINT)DIRECTION_TYPE::END];

public:
	tTransform* GetTransform() { return m_Transform; }
	XMMATRIX GetMatWorld() { return m_matWorld; }

	void SetScale(Vec4 _scale) { m_Transform->_Scale = _scale; }
	void SetRotation(Vec3 _rotation){ m_Transform->_Rotation = _rotation; }
	void SetPosition(Vec4 _position){ m_Transform->_Position = _position; }

	Vec4 GetScale() { return m_Transform->_Scale; }
	Vec3 GetRotation() { return m_Transform->_Rotation; }
	Vec4 GetPosition() { return  m_Transform->_Position; }

	Vec3 GetDir(DIRECTION_TYPE _type) { return m_Dir[(UINT)_type]; }
	void SetDir(DIRECTION_TYPE _type, Vec3 _dir) { m_Dir[(UINT)_type] = _dir; }
	void InitializeDir();

public:
	virtual void FinalTick() override;

public:
	Transform();
	virtual ~Transform();
};

