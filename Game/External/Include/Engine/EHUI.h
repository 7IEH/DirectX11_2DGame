#pragma once

#include "EHImGUIMgr.h"

class UI
{
private:
	string			m_label;
	const string	m_strID;
	bool			m_bActive;

	UI*				m_Parent;
	vector<UI*>		m_vecChildUI;
	
	bool			m_Collapsing;

public:
	void SetLabel(const string& _label) { m_label = _label; }
	const string& GetName() { return m_label; }
	const string& GetID() { return m_strID; }

	void AddChildUI(UI* _childUI)
	{
		m_vecChildUI.push_back(_childUI);
		_childUI->m_Parent = this;
	}

	UI* GetParentUI() { return m_Parent; }
	void Enabled(bool _active) { m_bActive = _active; }
	bool IsEnabled() { return m_bActive; }

	void SetCollapsing(bool _collapsing) { m_Collapsing = _collapsing; }

public:
	virtual void Update();
	virtual void Render();
	virtual void Render_Update() = 0;

public:
	UI(const string& _strName, const string& _strID);
	virtual ~UI();
};

