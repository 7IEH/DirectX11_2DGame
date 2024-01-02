#include "pch.h"
#include "EHImGUIMgr.h"

#include "EHDevice.h"

ImGUIMgr::ImGUIMgr()
	:m_Enabled(TRUE)
{
}

ImGUIMgr::~ImGUIMgr()
{
	ImGui_ImplDX11_Shutdown();
}

HRESULT ImGUIMgr::Awake()
{
	ImGui_ImplDX11_Init(DEVICE, CONTEXT);

	return TRUE;
}

void ImGUIMgr::Frame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGUIMgr::Render()
{
	Frame();
	if (m_Enabled)
	{
		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}