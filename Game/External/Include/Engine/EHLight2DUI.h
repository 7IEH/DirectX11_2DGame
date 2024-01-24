#pragma once

#include "EHComponentUI.h"

class Light2DUI
	:public ComponentUI
{
private:
	map<string, LIGHT_TYPE> m_NameByLayer;
	map<LIGHT_TYPE, string> m_LayerByName;

	vector<string>		m_LayerName;

	map<string, bool>	m_LayerSelect;

public:
	virtual void Render_Update() override;
public:
	Light2DUI();
	virtual ~Light2DUI();
};

