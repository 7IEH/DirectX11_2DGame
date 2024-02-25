#pragma once

#include <FW1FontWrapper.h>

class FontMgr
	:public Singleton<FontMgr>
{
	Single(FontMgr)
private:
	IFW1FontWrapper* m_FontWrapper;
	IDWriteFactory* m_WriteFactory;
	
public:
	void Awake();
	void DrawingText(wstring _str, UINT32 _color, wstring _font, DWRITE_FONT_WEIGHT _fontweight, float _x, float _y, float _fontsize = 32.0f);

private:
	HRESULT FontWrapperInit();
};

