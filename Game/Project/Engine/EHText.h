#pragma once
#include "EHComponent.h"

#include "EHFontMgr.h"

class Text :
	public Component
{
private:
	wstring m_sTextInput;
	wstring m_sFont;
	DWRITE_FONT_WEIGHT m_eWeight;
	UINT32 m_Color;
	float m_fFontSize;

public:
	virtual void Update();
	virtual void Render();

public:
	void SetText(wstring _str) { m_sTextInput = _str; }
	void SetColor(UINT32 _color) { m_Color = _color; }
	void SetFontWeight(DWRITE_FONT_WEIGHT _weight) { m_eWeight = _weight; }
	void SetFont(wstring _font) { m_sFont = _font; }
	void SetFontSize(float _size) { m_fFontSize = _size; }

	virtual void LateUpdate() {};

	virtual void Save(string _path) {};

	CLONE(Text)

public:
	Text();
	virtual ~Text();
};

