#include "pch.h"
#include "global.h"

void HandleError(HWND _hWnd, wstring _content, int flag)
{
	if (flag == 1)
	{
		MessageBoxW(_hWnd, _content.c_str(), L"INITIALIZE FAILED!", MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBoxW(_hWnd, _content.c_str(), L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
	}
}