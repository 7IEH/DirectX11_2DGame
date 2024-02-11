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
	

	bool						m_DefaultMouseFlags;
public:
	virtual void Render_Update() override;

private:
	void Render_File();
	void Render_FileIcon();

	void Mouse_Event();
	void EnterFilePath(string _filepath, bool _directory);

	void Create_SceneFile();
	void Load_Scene(string _sceneName);
public:
	ProjectView();
	virtual ~ProjectView();
};

