#pragma once
#include "EHComponentUI.h"
class ScriptUI :
	public ComponentUI
{
private:
	string m_ScriptName;

public:
	virtual void Render_Update()override;
	
public:
	string GetName() { return m_ScriptName; }

public:
	ScriptUI(string _scriptName);
	virtual ~ScriptUI();
};

