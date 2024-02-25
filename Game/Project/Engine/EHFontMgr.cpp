#include "pch.h"
#include "EHFontMgr.h"

#include "EHDevice.h"
#include <FW1FontWrapper.h>

FontMgr::FontMgr()
{

}

FontMgr::~FontMgr()
{
	m_FontWrapper->Release();
}

void FontMgr::Awake()
{
	if (FAILED(FontWrapperInit()))
	{
		HandleError(MAIN_HWND, L"FontMgr Awake Error", 0);
		assert(nullptr);
	}

	e_Font = L"DroidSansFallback";
}

void FontMgr::DrawingText(wstring _str, UINT32 _color, wstring _font, DWRITE_FONT_WEIGHT _fontweight, float _x, float _y, float _fontsize)
{
	IDWriteTextFormat* _textFormat;
	HRESULT _result = m_WriteFactory->CreateTextFormat(
		e_Font.c_str(),
		NULL,
		_fontweight,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_fontsize,
		L"",
		&_textFormat
	);

	IDWriteTextLayout* pTextLayout = nullptr;
	HRESULT hResult = m_WriteFactory->CreateTextLayout(
		_str.c_str(),
		_str.size(),
		_textFormat,
		0.f,
		0.0f,
		&pTextLayout
	);
	pTextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	m_FontWrapper->DrawTextLayout(CONTEXT, pTextLayout, _x, _y, _color, 0);
}

HRESULT FontMgr::FontWrapperInit()
{
	// Create the font-wrapper
	IFW1Factory* _FontFactory = nullptr;
	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &_FontFactory);
	if (FAILED(hResult)) {
		MessageBox(NULL, TEXT("FW1CreateFactory"), TEXT("Error"), MB_OK);
		return 0;
	}

	hResult = _FontFactory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper);
	if (FAILED(hResult)) {
		MessageBox(NULL, TEXT("CreateFontWrapper"), TEXT("Error"), MB_OK);
		return 0;
	}

	_FontFactory->Release();

	hResult = m_FontWrapper->GetDWriteFactory(&m_WriteFactory);

	return S_OK;
}