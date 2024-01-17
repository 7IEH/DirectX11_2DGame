#pragma once

#include "EHUI.h"

class Inspector;
class Hierarchy
	:public UI
{
private:
	Inspector* m_Inspector;
	
public:
	void SetInspector(Inspector* _inspector) { m_Inspector = _inspector; }

public:
	virtual void Render_Update();

public:
	Hierarchy();
	virtual ~Hierarchy();
};

