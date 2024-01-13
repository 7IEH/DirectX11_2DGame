#pragma once

class ImGUIMgr
	:public Singleton<ImGUIMgr>
{
	Single(ImGUIMgr)
private:
	bool		m_Enabled;
	FONT_TYPE	m_Type;

	ImGuiIO&		m_io;

	bool		m_Check;

	bool		m_DockSpace;

	// Console Message �Ŀ� �������� �� �ش� ������ ��ȣ �����°� ã�ƺ���
	vector<string> m_ConsoleMessage;

	float		m_AcctimeforDebug;
	float		m_OutputTime;

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
	void	ShowDockSpace();

	void	ChangeFont(FONT_TYPE _type);
	void	ChangeFontIndividual(string _type, float _size);

	/******************
	|	Menu
	******************/
	void	GameView();
	void	InSpector();
	void	Hierarchy();
	void	Console();
};

