#pragma once

#include "EHUI.h"

class ImGUIMgr
	:public Singleton<ImGUIMgr>
{
	Single(ImGUIMgr)
private:
	map<string, UI*> m_mapUI;

	bool		m_Enabled;
	FONT_TYPE	m_Type;

	ImGuiIO&		m_io;

	bool		m_DockSpace;
	
	bool		m_SpriteEditor;

	UI*			m_Sprite;

	// Console Message 후에 선생님이 한 해당 페이지 번호 가져온거 찾아보기

	GameObject* m_Inspector;

public:
	void Enabled() { m_Enabled = !m_Enabled; }
	bool IsEnabled() { return m_Enabled; }

	ImGuiIO GetIO() { return m_io; }
	void Frame();
public:
	HRESULT Awake();
	void Render();

private:
	void CreateUI();

public:
	UI* FindUI(const string& _label);
	void AddUI(const string& _label, UI* _ui);

private:
	void	ShowDockSpace();

	void	ChangeFont(FONT_TYPE _type);
	void	ChangeFontIndividual(string _type, float _size);
};

