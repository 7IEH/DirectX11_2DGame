#include "pch.h"
#include "EHImGUIMgr.h"

#include "EHDevice.h"
#include "EHPathMgr.h"

#include "EHGameObject.h"
#include "EHBehaviour.h"

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
	m_io = ImGui::GetIO();
	ChangeFont(FONT_TYPE::Maple);
	return TRUE;
}

void ImGUIMgr::ChangeFont(FONT_TYPE _type)
{
	wstring _localPath = PATH;
	switch (_type)
	{
	case FONT_TYPE::Default:
	{
		m_io.Fonts->AddFontDefault();
	}
	break;
	case FONT_TYPE::Maple:
	{
		_localPath += L"\\fonts\\Maplestory Light.ttf";
		string _temp;
		_temp.assign(_localPath.begin(), _localPath.end());
		ChangeFontIndividual(_temp, 18.0f);
	}
	break;
	case FONT_TYPE::END:
		break;
	default:
		break;
	}
}

void ImGUIMgr::ChangeFontIndividual(string _type, float _size)
{
	ImFont* _font = m_io.Fonts->AddFontFromFileTTF(_type.c_str(), _size);
}

void ImGUIMgr::Frame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGUIMgr::Render()
{

	if (m_Player == nullptr)
		return;

	Frame();
	if (m_Enabled)
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
		Transform* _tr = m_Player->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		Vec4 _pos = _tr->GetRelativePosition();
		Vec4 _scale = _tr->GetRelativeScale();
		Vec3 _rot = _tr->GetRelativeRotation();

		ImGui::Begin("Character Inspector");

		ImGui::Text("Position");
		ImGui::InputScalar("PosX", ImGuiDataType_Float, &_pos.x, &_pos.x, &_pos.x, "%.f", flags);
		ImGui::InputScalar("PosY", ImGuiDataType_Float, &_pos.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("PosZ", ImGuiDataType_Float, &_pos.z, 0, 0, "%.f", flags);

		ImGui::Text("Scale");
		ImGui::InputScalar("ScaleX", ImGuiDataType_Float, &_scale.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("ScaleY", ImGuiDataType_Float, &_scale.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("ScaleZ", ImGuiDataType_Float, &_scale.z, 0, 0, "%.f", flags);

		ImGui::Text("Rotation");
		ImGui::InputScalar("RotX", ImGuiDataType_Float, &_rot.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("RotY", ImGuiDataType_Float, &_rot.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("RotZ", ImGuiDataType_Float, &_rot.z, 0, 0, "%.f", flags);

		_tr->SetRelativePosition(_pos);
		_tr->SetRelativeScale(_scale);
		_tr->SetRelativeRotation(_rot);

		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}