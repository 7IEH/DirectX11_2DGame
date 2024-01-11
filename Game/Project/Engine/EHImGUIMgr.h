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

	GameObject* m_Player;
	GameObject* m_Light;

	bool		m_DockSpace;

	ComPtr<ID3D11ShaderResourceView> _SRV;
	ComPtr<ID3D11Texture2D> textureCopy;

public:
	void Enabled() { m_Enabled = !m_Enabled; }
	bool IsEnabled() { return m_Enabled; }

	ImGuiIO GetIO() { return m_io; }
	void Frame();

	void SetObject(GameObject* _Player) { m_Player = _Player; }
	GameObject* GetObjectPlayer() { return m_Player; }

	void SetLight(GameObject* _light) { m_Light = _light; }
	GameObject* GetObjectLight() { return m_Light; }
public:
	HRESULT Awake();
	void Render();

private:
	void	ShowDockSpace();

	void	ChangeFont(FONT_TYPE _type);
	void	ChangeFontIndividual(string _type, float _size);
};

