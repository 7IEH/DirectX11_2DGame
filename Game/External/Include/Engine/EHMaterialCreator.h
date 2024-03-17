#pragma once
#include "EHUI.h"
class MaterialCreator :
	public UI
{
private:
	string m_sName;
	string m_sShader;
	string m_sSprite;

	wstring m_wName;
	wstring m_wShader;
	wstring m_wSprite;

public:
	virtual void Render_Update()override;

public:
	MaterialCreator();
	~MaterialCreator();
};

