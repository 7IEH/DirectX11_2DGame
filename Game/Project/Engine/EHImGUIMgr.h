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
	bool		m_CollisionMatrix;
	bool		m_bAnimationUI;
	bool		m_bSpriteLoader;
	bool		m_bunAnimationCreateUI;
	bool		m_bTilePalette;
	bool		m_bCreateSceneUI;
	bool		m_bDeleteUI;

	//
	UI*			m_Sprite;
	UI*			m_CollisionMat;
	UI*			m_AnimationCreateUI;
	UI*			m_Hierarchy;
	UI*			m_SpriteLoader;
	UI*			m_unAnimationCreateUI;
	UI*			m_TilePalette;
	UI*			m_CreateSceneUI;
	UI*			m_DeleteUI;

	// Console Message 후에 선생님이 한 해당 페이지 번호 가져온거 찾아보기

	GameObject* m_Inspector;

public:
	void Enabled() { m_Enabled = !m_Enabled; }
	bool IsEnabled() { return m_Enabled; }

	ImGuiIO GetIO() { return m_io; }
	void Frame();

	UI* GetAnimationUI() { return m_AnimationCreateUI; }

	void SetAnimationUI(bool _animationUI) { m_bAnimationUI = _animationUI; }

	void SetSprite(bool _flag) { m_SpriteEditor = _flag; }
	void SetCol(bool _flag) { m_CollisionMatrix = _flag; }
	void SetAnimationCreate(bool _flag) { m_bAnimationUI = _flag; }
	void SetSpriteLoader(bool _flag) { m_bSpriteLoader = _flag; }
	void SetunAnimationCreateUI(bool _flag) { m_bunAnimationCreateUI = _flag; }
	void SetTilePalette(bool _flag) { m_bTilePalette = _flag; }
	void SetCreateSceneUI(bool _flag) { m_bCreateSceneUI = _flag; }
	void SetDeleteUI(bool _flag) { m_bDeleteUI = _flag; }
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

