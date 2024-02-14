#pragma once
#include "EHUI.h"
class ProjectView :
	public UI
{
private:
	string						m_AbsolutePath;
	string						m_RelativePath;
	string						m_ColumnPath;
	std::filesystem::path		m_FileSystemPath;
	
	bool						m_IsHovered;
	bool						m_DefaultMouseFlags;
public:
	virtual void Render_Update() override;

private:
	void Render_File();
	void Render_FileIcon();

	void Mouse_Event();
	void EnterFilePath(string _filepath, bool _directory);

	void Create_Object();
	void Create_SceneFile();
	void Load_Scene(string _sceneName);

	void Delete_Scene();
	void Delete_Object();
public:
	ProjectView();
	virtual ~ProjectView();
};

