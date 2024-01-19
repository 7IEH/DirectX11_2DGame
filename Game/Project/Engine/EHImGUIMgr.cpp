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

#include "EHGameView.h"
#include "EHHierarchy.h"
#include "EHInspector.h"
#include "EHConsole.h"
#include "EHSceneView.h"
#include "EHSpriteEditor.h"

ImGUIMgr::ImGUIMgr()
	: m_Enabled(TRUE)
	, m_DockSpace(TRUE)
	, m_io(ImGui::GetIO())
	, m_SpriteEditor(FALSE)
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
	
	CreateUI();

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
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	/*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/
}

void ImGUIMgr::CreateUI()
{
	UI* pUI = nullptr;
	// 1. Hierarchy
	pUI = new Hierarchy;
	AddUI("Hierarchy", pUI);

	// 2. Console
	pUI = new Console;
	AddUI("Console", pUI);

	// 3. Inspector
	pUI = new Inspector;
	AddUI("Inspector", pUI);

	// 4. GameView
	pUI = new GameView;
	AddUI("GameView", pUI);

	// 5. SceneView
	pUI = new SceneView;
	AddUI("Scene", pUI);

	// 6. SpriteEditor
	pUI = new SpriteEditor;
	AddUI("SpriteEditor", pUI);

	m_Sprite = pUI;

	// Inspector apply
	dynamic_cast<Hierarchy*>(FindUI("Hierarchy"))->SetInspector(dynamic_cast<Inspector*>(FindUI("Inspector")));
}

UI* ImGUIMgr::FindUI(const string& _label)
{
	map<string, UI*>::iterator iter = m_mapUI.find(_label);

	if (iter == m_mapUI.end())
	{
		return nullptr;
	}

	return iter->second;
}

void ImGUIMgr::AddUI(const string& _label, UI* _ui)
{
	UI* _pui = FindUI(_label);
	assert(!_pui);
	m_mapUI.insert(make_pair(_label, _ui));
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
		if (ImGui::BeginMenu("Utility"))
		{
			ImGui::MenuItem("SpriteRenderer", "", &m_SpriteEditor);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_Sprite->Enabled(m_SpriteEditor);

	for (const auto& pair : m_mapUI)
	{
		pair.second->Update();
	}

	for (const auto& pair : m_mapUI)
	{
		pair.second->Render();
	}

	ImGui::End();
}