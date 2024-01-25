#pragma once
#include "EHComputeShader.h"

#include "EHSprite.h"

class TestComputeShader :
	public ComputeShader
{
private:
	Ptr<Sprite> m_TargetTex;
	Vec4        m_Color;

public:
	void SetTargetTexture(Ptr<Sprite> _tex) { m_TargetTex = _tex; }
	void SetColor(Vec3 _color) { m_Color = Vec4(_color.x, _color.y, _color.z, 1.f); }

public:
	virtual HRESULT UpdateData() override;
	virtual void UpdateGroupCount() override;
	virtual void Clear() override;

public:
	TestComputeShader();
	virtual ~TestComputeShader();
};

