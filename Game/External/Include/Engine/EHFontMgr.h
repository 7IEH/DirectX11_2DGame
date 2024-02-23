#pragma once

#include <FW1FontWrapper.h>

class FontMgr
	:public Singleton<FontMgr>
{
	Single(FontMgr)
private:
	IFW1FontWrapper*		m_FontWrapper;
	IFW1TextGeometry*		m_TextGeoMetry;
	IDWriteFactory*			m_WriteFactory;
	
public:
	void Awake();
	void DrawingText(wstring _str);

private:
	HRESULT FontWrapperInit();
};

