#pragma once
#include "EHComponentUI.h"
class CameraUI :
	public ComponentUI
{
private:
	map<string, PROJECTION_TYPE> m_NameByLayer;
	map<PROJECTION_TYPE, string> m_LayerByName;

	vector<string>		m_LayerName;

	map<string, bool>	m_LayerSelect;

	map<UINT, string> m_VisibleLayer;

public:
	virtual void Render_Update()override;

public:
	CameraUI();
	virtual ~CameraUI();
};

