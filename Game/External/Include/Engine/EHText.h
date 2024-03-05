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
	Vec4 m_Color;
	float m_fFontSize;

public:
	virtual void Update();
	virtual void Render();

	virtual void LateUpdate() {};

	virtual void Save(string _path);
	virtual void Load(std::wifstream* _file);

public:
	void SetText(wstring _str) { m_sTextInput = _str; }
	void SetColor(Vec4 _color) { m_Color = _color; }
	void SetFontWeight(DWRITE_FONT_WEIGHT _weight) { m_eWeight = _weight; }
	void SetFont(wstring _font) { m_sFont = _font; }
	void SetFontSize(float _size) { m_fFontSize = _size; }

	wstring GetText() { return  m_sTextInput; }
	Vec4 GetColor() { return m_Color; }
	DWRITE_FONT_WEIGHT GetFontWeight() { return m_eWeight; }
	wstring GetFont() { return m_sFont; }
	float GetFontSize() { return m_fFontSize; }

	CLONE(Text)

public:
	Text();
	virtual ~Text();
};

