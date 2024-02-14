#pragma once
#include "EHUI.h"
class DeleteUI :
	public UI
{
private:
	string m_GameOjbect;

public:
	virtual void Render_Update() override;

public:
	DeleteUI();
	virtual ~DeleteUI();
};

