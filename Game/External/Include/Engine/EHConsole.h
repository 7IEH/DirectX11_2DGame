#pragma once

#include "EHUI.h"

class Console
	:public UI
{
private:
	vector<string> m_ConsoleMessage;

	float		m_AcctimeforDebug;
	float		m_OutputTime;

public:
	virtual void Render_Update() override;

public:
	Console();
	virtual ~Console();
};

