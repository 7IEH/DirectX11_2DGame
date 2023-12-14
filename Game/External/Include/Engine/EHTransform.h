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

public:
	virtual void Tick() override;

public:
	Transform();
	virtual ~Transform();
};

