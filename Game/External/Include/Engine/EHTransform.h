#pragma once

#include "EHComponent.h"

class Transform
	: public Component
{
private:
	tTransform* m_Transform;
	transform* m_Matrix;

public:
	tTransform* GetTransform() { return m_Transform; }
	transform* GetMatrix() { return m_Matrix; }

	void SetScale(Vec4 _scale) { m_Transform->_Scale = _scale; }
	void SetRotation(float _rotation){ m_Transform->_Rotation = _rotation; }
	void SetPosition(Vec4 _position){ m_Transform->_Position = _position; }

	Vec4 SetScale() { return m_Transform->_Scale; }
	float SetRotation() { return m_Transform->_Rotation; }
	Vec4 SetPosition() { return  m_Transform->_Position; }

public:
	virtual void Tick() override;

public:
	Transform();
	virtual ~Transform();
};

