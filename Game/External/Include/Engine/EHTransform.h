#pragma once

#include "EHComponent.h"

class Transform
	: public Component
{
private:
	ComPtr<ID3D11Buffer> m_CB;

	tTransform* m_Trasnform;

public:
	Transform();
	virtual ~Transform();

public:
	tTransform* GetTransform() { return m_Trasnform; }

public:
	virtual void Create() override;
	virtual void Tick() override;
	virtual void Render() override {};

private:
	void CreateConstantBuffer();
};

