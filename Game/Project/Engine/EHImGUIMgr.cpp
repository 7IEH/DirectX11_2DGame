#include "pch.h"
#include "EHImGUIMgr.h"

#include "EHDevice.h"
#include "EHPathMgr.h"
#include "EHAssetMgr.h"
#include "EHRenderMgr.h"
#include "EHLevelMgr.h"

#include "EHGameObject.h"
#include "EHBehaviour.h"

#include "EHDebugMgr.h"
#include "EHTimeMgr.h"

#include "EHSprite.h"

ImGUIMgr::ImGUIMgr()
	: m_Enabled(TRUE)
	, m_DockSpace(TRUE)
	, m_io(ImGui::GetIO())
	, m_OutputTime(1.5f)
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
	Frame();
	ShowDockSpace();

	//ImGui::ShowDemoWindow()

	//Frame();
	//if (m_Enabled)
	//{
	//	ImGui::ShowDemoWindow(&m_Enabled);
	//}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/***************
|	DockSpace
|	1. GameView
|	2. Hierarchy
|	3. Console
|	DockSPace
***************/

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

	InSpector();

	GameView();

	Hierarchy();

	Console();

	ImGui::End();
}

void ImGUIMgr::GameView()
{
	ImGui::Begin("Game");
	ImGui::Image((void*)RenderMgr::GetInst()->GetPostProcessTexture2D().Get()->GetSRV().Get(), ImVec2(640.f, 360.f));
	ImGui::End();
}

void ImGUIMgr::InSpector()
{
	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	bool enabled = FALSE;
	for (size_t _layer = 0; _layer < (UINT)LAYER_TYPE::END;_layer++)
	{
		Layer* _layers = _curLevel->GetLayer(LAYER_TYPE(_layer));
		vector<GameObject*>& _objs = _layers->GetLayerObject();

		for (size_t _obj = 0; _obj < _objs.size();_obj++)
		{
			if (_objs[_obj]->GetPicking() == TRUE)
			{
				m_Inspector = _objs[_obj];
				enabled = TRUE;
				break;
			}
		}

		if (enabled)
			break;
	}

	if (nullptr == m_Inspector)
		return;

	Transform* _tr = m_Inspector->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	MeshRenderer* _renderer = m_Inspector->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER);
	Animator2D* _animator = m_Inspector->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
	Collider2D* _collider = m_Inspector->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D);
	LIght2D* _light2D = m_Inspector->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);
	// Script*
	// Light*
	// Camera*

	ImGui::Begin("Inspector");
	if (_tr != nullptr)
	{
		if (ImGui::CollapsingHeader("Transoform"))
		{
			Vec4 _scale = _tr->GetRelativeScale();
				Vec4 _pos = _tr->GetRelativePosition();
				Vec3 _rot = _tr->GetRelativeRotation();

				float _value[3] = { _pos.x,_pos.y,_pos.z };
				ImGui::InputFloat3("Position", _value);
				float _value2[3] = { _rot.x,_rot.y,_rot.z };
				ImGui::InputFloat3("Rotation", _value2);
				float _value3[3] = { _scale.x,_scale.y,_scale.z };
				ImGui::InputFloat3("Scale", _value3);

			_pos.x = _value[0];
			_pos.y = _value[1];
			_pos.z = _value[2];

			_rot.x = _value2[0];
			_rot.y = _value2[1];
			_rot.z = _value2[2];

			_scale.x = _value3[0];
			_scale.y = _value3[1];
			_scale.z = _value3[2];

			_tr->SetRelativePosition(_pos);
			_tr->SetRelativeRotation(_rot);
			_tr->SetRelativeScale(_scale);
		}
	}

	if (_renderer != nullptr)
	{
		
	}

	if (_light2D != nullptr)
	{
		if (ImGui::CollapsingHeader("Light2D"))
		{
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;

			LIGHT_TYPE _type = _light2D->GetLightType();
			float _typeInfo = (float)_type;
			float _Radius = _light2D->GetRadius();
			float _Angle = _light2D->GetAngle();
			Vec4 _Color = _light2D->GetColor();
			Vec4 _Ambient = _light2D->GetAmbient();

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

			_light2D->SetAmbient(_Ambient);
			_light2D->SetColor(_Color);

			int _push = (int)_typeInfo;
			_light2D->SetLightType(LIGHT_TYPE(_push));
			_light2D->SetRadius(_Radius);
			_light2D->SetAngle(_Angle);
		}
	}

	ImGui::End();
}

/*****************
|	Object Picking
*****************/

void ImGUIMgr::Hierarchy()
{
	ImGui::Begin("Hierarchy");
	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	static int testidx = 0;

	for (size_t _idx = 0;_idx < (UINT)LAYER_TYPE::END;_idx++)
	{
		Layer* _layer = _curLevel->GetLayer(LAYER_TYPE(_idx));
		vector<GameObject*>& _objs = _layer->GetLayerObject();

		for (size_t _obj = 0;_obj < _objs.size();_obj++)
		{
			wstring _name_w = _objs[_obj]->GetName();
			string _name;

			bool _picking = _objs[_obj]->GetPicking();
			_name.assign(_name_w.begin(), _name_w.end());

			ImGui::Selectable(_name.c_str(), &_picking);

			_objs[_obj]->SetPicking(_picking);
		}
	}

	ImGui::End();
}

void ImGUIMgr::Console()
{
	ImGui::Begin("Console");

	vector<string> _msg = DebugMgr::GetInst()->GetDebugMessage();

	for (int i = 0;i < _msg.size();i++)
	{
		m_ConsoleMessage.push_back(_msg[i]);
	}

	static int _line = 0;
	m_AcctimeforDebug += DT;
	if (m_AcctimeforDebug >= m_OutputTime)
	{
		_line++;

		if (_line >= m_ConsoleMessage.size())
		{
			_line = m_ConsoleMessage.size();
		}

		m_AcctimeforDebug = 0.f;
	}

	ImGui::BeginChild("Scrolling");
	for (int i = 0;i < _line;i++)
	{
		ImGui::Text(m_ConsoleMessage[i].c_str());
	}
	ImGui::EndChild();

	ImGui::End();
}