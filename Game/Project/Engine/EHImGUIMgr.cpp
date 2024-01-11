#include "pch.h"
#include "EHImGUIMgr.h"

#include "EHDevice.h"
#include "EHPathMgr.h"
#include "EHAssetMgr.h"
#include "EHRenderMgr.h"

#include "EHGameObject.h"
#include "EHBehaviour.h"

#include "EHSprite.h"

ImGUIMgr::ImGUIMgr()
	:m_Enabled(TRUE)
	,m_DockSpace(TRUE)
	,m_io(ImGui::GetIO())
{
}

ImGUIMgr::~ImGUIMgr()
{
	ImGui_ImplDX11_Shutdown();
}

HRESULT ImGUIMgr::Awake()
{
	ImGui_ImplDX11_Init(DEVICE, CONTEXT);
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
	if (m_DockSpace)
	{
		m_io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
		m_io.ConfigDockingAlwaysTabBar = TRUE;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGUIMgr::Render()
{
	if (m_Player == nullptr && m_Light == nullptr)
		return;

	Frame();
	ShowDockSpace();
	
	/*Frame();
	if (m_Enabled)
	{
		ImGui::ShowDemoWindow(&m_Enabled);
	}*/

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGUIMgr::ShowDockSpace()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &m_DockSpace, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
			if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
			if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, m_DockSpace != NULL))
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (m_Enabled && m_Player != nullptr)
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
	else if (m_Enabled && m_Light != nullptr)
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
		Transform* _tr = m_Light->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		LIght2D* _light = m_Light->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);

		Vec4 _pos = _tr->GetRelativePosition();
		Vec3 _rot = _tr->GetRelativeRotation();

		LIGHT_TYPE _type = _light->GetLightType();
		float _typeInfo = (float)_type;
		float _Radius = _light->GetRadius();
		float _Angle = _light->GetAngle();
		Vec4 _Color = _light->GetColor();
		Vec4 _Ambient = _light->GetAmbient();

		ImGui::Begin("Light Inspector");

		ImGui::Text("Position");
		ImGui::InputScalar("PosX", ImGuiDataType_Float, &_pos.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("PosY", ImGuiDataType_Float, &_pos.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("PosZ", ImGuiDataType_Float, &_pos.z, 0, 0, "%.f", flags);

		ImGui::Text("Rotation");
		ImGui::InputScalar("RotX", ImGuiDataType_Float, &_rot.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("RotY", ImGuiDataType_Float, &_rot.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("RotZ", ImGuiDataType_Float, &_rot.z, 0, 0, "%.f", flags);

		ImGui::Text("LightSetting");
		ImGui::InputScalar("Radius", ImGuiDataType_Float, &_Radius, 0, 0, "%.f", flags);
		ImGui::InputScalar("Angle", ImGuiDataType_Float, &_Angle, 0, 0, "%.f", flags);

		ImGui::Text("LightColor");
		ImGui::InputScalar("RED", ImGuiDataType_Float, &_Color.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("BLUE", ImGuiDataType_Float, &_Color.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("GREEN", ImGuiDataType_Float, &_Color.z, 0, 0, "%.f", flags);

		ImGui::Text("LightAmbient");
		ImGui::InputScalar("RED_A", ImGuiDataType_Float, &_Ambient.x, 0, 0, "%.f", flags);
		ImGui::InputScalar("BLUE_A", ImGuiDataType_Float, &_Ambient.y, 0, 0, "%.f", flags);
		ImGui::InputScalar("GREEN_A", ImGuiDataType_Float, &_Ambient.z, 0, 0, "%.f", flags);

		ImGui::Text("Type");
		ImGui::InputScalar("RIGHTTYPE", ImGuiDataType_Float, &_typeInfo, 0, 0, "%.f", flags);

		_light->SetAmbient(_Ambient);
		_light->SetColor(_Color);

		int _push = (int)_typeInfo;
		_light->SetLightType(LIGHT_TYPE(_push));
		_light->SetRadius(_Radius);
		_light->SetAngle(_Angle);

		_tr->SetRelativePosition(_pos);
		_tr->SetRelativeRotation(_rot);

		ImGui::End();
	}

	ImGui::Begin("Test");
	ImGui::Image((void*)RenderMgr::GetInst()->GetPostProcessTexture2D().Get()->GetSRV().Get(), ImVec2(1280.f, 720.f));
	ImGui::End();

	ImGui::End();
}