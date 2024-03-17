#include "pch.h"
#include "EHSceneCreateUI.h"

#include "EHPathMgr.h"
#include "EHImGUIMgr.h"
#include "EHLevelMgr.h"

SceneCreateUI::SceneCreateUI()
	:UI("SceneCreateUI", "##SceneCreateUI")
{
}

SceneCreateUI::~SceneCreateUI()
{
}

void SceneCreateUI::Render_Update()
{
	ImGui::InputText("##SceneCreateUI", &m_SceneName);
	ImGui::SameLine();
	if (ImGui::Button("Create"))
	{
		wstring _absPath = PathMgr::GetInst()->GetPath();
		string _filePath = string(_absPath.begin(), _absPath.end());
		_filePath += "\\Assets\\Scenes\\" + m_SceneName + ".Scene";
		File_Create(_filePath);
		LevelMgr::GetInst()->AddLevel<Level>(EH::ConvertWstring(m_SceneName));

		m_SceneName = "";
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		wstring _absPath = PathMgr::GetInst()->GetPath();
		string _filePath = string(_absPath.begin(), _absPath.end());
		_filePath += "\\Assets\\Scenes\\" + m_SceneName + ".Scene";
		File_Delete(_filePath);
		LevelMgr::GetInst()->DeletLevel(EH::ConvertWstring(m_SceneName));

		m_SceneName = "";
	}
	ImGui::SameLine();
}

void SceneCreateUI::File_Create(string _filepath)
{
	std::ofstream(_filepath.c_str());
}

void SceneCreateUI::File_Delete(string _filepath)
{
	std::filesystem::remove_all(_filepath);
}