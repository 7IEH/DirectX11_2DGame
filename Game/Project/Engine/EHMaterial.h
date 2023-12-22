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

	material* GetMaterialData() { return m_tMaterial; }
	void SetMaterialData(material* _tMaterial) { m_tMaterial = _tMaterial; }

public:
	Material();
	virtual ~Material();
};

