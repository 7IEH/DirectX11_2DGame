#pragma once
#include "EHAsset.h"

class GraphicShader;
class Sprite;

class Material
	:public Asset
{
private:
	material*				m_tMaterial;
	GraphicShader*			m_GraphicShader;

	Sprite*					m_Sprite[(UINT)TEX_PARAM::END];

public:
	virtual void UpdateData();
	virtual void Render();

public:
	GraphicShader* GetGraphicShader() { return m_GraphicShader; }
	void SetGraphicShader(GraphicShader* _graphicShader) { m_GraphicShader = _graphicShader; }

	material* GetMaterialData() { return m_tMaterial; }
	void SetMaterialData(material* _tMaterial) { m_tMaterial = _tMaterial; }

public:
	Material();
	virtual ~Material();
};

