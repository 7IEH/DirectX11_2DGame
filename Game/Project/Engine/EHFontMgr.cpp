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
}

void FontMgr::DrawingText(wstring _str)
{
	IDWriteTextFormat* pTextFormat;
	HRESULT hResult = m_WriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_OBLIQUE,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"",
		&pTextFormat
	);

	IDWriteTextLayout* pTextLayout;
	const WCHAR str[] = L"\"My Text Layout String\"\n\n123 !!!\n~~\n__ test";
	hResult = m_WriteFactory->CreateTextLayout(
		str,
		sizeof(str) / sizeof(str[0]),
		pTextFormat,
		0.0f,
		0.0f,
		&pTextLayout
	);
	pTextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);


	m_FontWrapper->DrawTextLayout(CONTEXT, pTextLayout, 0.5, 0.5, 0xffffffff, 0);

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

	// Create the default DWrite text format for the text layout
	IDWriteTextFormat* pTextFormat;
	hResult = m_WriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_OBLIQUE,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"",
		&pTextFormat
	);

	// Create a text layout for a string



	m_WriteFactory->Release();

	return S_OK;
}
