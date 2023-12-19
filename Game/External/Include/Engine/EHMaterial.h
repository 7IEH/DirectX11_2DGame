#pragma once
#include "EHAsset.h"

class Material
	:public Asset
{
private:
	material* m_tMaterial;

public:
	virtual void UpdateData();
	virtual void Render();

public:
	Material();
	virtual ~Material();
};

