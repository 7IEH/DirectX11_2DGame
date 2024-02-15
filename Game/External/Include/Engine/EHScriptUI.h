#pragma once
#include "EHComponentUI.h"
class ScriptUI :
	public ComponentUI
{
private:

public:
	virtual void Render_Update()override;

public:
	ScriptUI();
	virtual ~ScriptUI();
};

