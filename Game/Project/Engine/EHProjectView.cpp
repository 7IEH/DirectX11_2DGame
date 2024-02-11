#include "pch.h"
#include "EHProjectView.h"

#include "EHPathMgr.h"
#include "EHLevelMgr.h"

#include "EHImGUIMgr.h"

ProjectView::ProjectView()
	:UI("ProjectView", "##ProjectView")
	, m_DefaultMouseFlags(FALSE)
{
	wstring _path = PathMgr::GetInst()->GetPath();
	m_AbsolutePath = string(_path.begin(), _path.end());
	m_RelativePath = "";
	m_ColumnPath = "";
	m_FileSystemPath = std::filesystem::path(m_AbsolutePath);
}

ProjectView::~ProjectView()
{
}

void ProjectView::Render_Update()
{
	// 열 사이즈 조정
	static ImGuiTableFlags _flags = ImGuiTableFlags_Resizable;

	// 테이블 표 시작
	ImGui::BeginTable("ProjectViewTable", 2, _flags);

	Mouse_Event();

	// 각 테이블 표 주석 이름
	ImGui::TableSetupColumn("File");
	ImGui::TableSetupColumn(m_ColumnPath.c_str());
	ImGui::TableHeadersRow();

	// 첫번째 열 Render : File이름
	Render_File();

	// 두번째 열 Render : File안에 파일들 
	Render_FileIcon();

	ImGui::EndTable();
}

void ProjectView::Render_File()
{
	string _UpperFile = m_AbsolutePath.substr(m_AbsolutePath.find_last_of("content") - 6);

	int _row = 0;

	// File
	ImGui::TableNextRow();
	ImGui::Text("");
	ImGui::TableSetColumnIndex(0);


	// 두번째 열 header 나태내는 string
	m_ColumnPath = "";
	m_RelativePath = "";
	if (ImGui::TreeNode(_UpperFile.c_str()))
	{
		m_ColumnPath += _UpperFile;
		for (const std::filesystem::directory_entry& _entry :
			std::filesystem::directory_iterator(m_FileSystemPath))
		{
			EnterFilePath(string(_entry.path().native().begin(), _entry.path().native().end()), _entry.is_directory());
		}

		ImGui::TreePop();
	}
}

void ProjectView::EnterFilePath(string _filepath, bool _directory)
{
	// File Name
	string _UpperName = _filepath.substr(_filepath.find_last_of('\\') + 1);

	// File Path
	std::filesystem::path _path(_filepath);

	if (_directory)
	{
		if (ImGui::TreeNode(_UpperName.c_str()))
		{
			m_ColumnPath += " > " + _UpperName;
			m_RelativePath += "\\" + _UpperName;
			for (const std::filesystem::directory_entry& _entry :
				std::filesystem::directory_iterator(_path))
			{
				EnterFilePath(string(_entry.path().native().begin(), _entry.path().native().end()), _entry.is_directory());
			}
			ImGui::TreePop();
		}
	}
}

void ProjectView::Render_FileIcon()
{
	int _row = 0;
	ImGui::TableSetColumnIndex(1);

	string _currentPath = m_AbsolutePath + m_RelativePath;
	//File Icon
	for (const std::filesystem::directory_entry& _entry :
		std::filesystem::directory_iterator(_currentPath))
	{
		if (!_entry.is_directory())
		{
			string _UpperName = EH::ConvertString(_entry.path().native());
			_UpperName = _UpperName.substr(_UpperName.find_last_of('\\') + 1);
			// 디렉토리

			if (ImGui::Selectable(_UpperName.c_str()))
			{
				Load_Scene(_UpperName);
			}
		}
	}
}

void ProjectView::Mouse_Event()
{
	ImGui::InvisibleButton("ProjectViewTable", ImVec2(1600.f, 900.f)
		, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || m_DefaultMouseFlags)
	{
		ImGui::OpenPopup("ProjectViewOption1");
		m_DefaultMouseFlags = TRUE;
		if (ImGui::BeginPopup("ProjectViewOption1"))
		{
			bool _flags = FALSE;
			if (ImGui::MenuItem("Create", NULL, &_flags))
			{
				// Scene 제작
				Create_SceneFile();

				m_DefaultMouseFlags = FALSE;
			}

			ImGui::EndPopup();
		}
	}
}

void ProjectView::Create_SceneFile()
{
	ImGUIMgr::GetInst()->SetCreateSceneUI(TRUE);
}

void ProjectView::Load_Scene(string _sceneName)
{
	wstring _wsceneName = wstring(_sceneName.begin(), _sceneName.end());
	_wsceneName = _wsceneName.substr(0, _wsceneName.find(L"."));

	LevelMgr::GetInst()->SelectLevel(_wsceneName);
}