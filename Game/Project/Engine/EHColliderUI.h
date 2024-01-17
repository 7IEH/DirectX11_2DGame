#pragma once
#include "EHComponentUI.h"

class ColliderUI
	:public ComponentUI
{
private:
	COLLIDER_TYPE m_Type;

public:
	virtual void Render_Update();

public:
	ColliderUI(const string& _label, const string& _strID, COLLIDER_TYPE _type);
	virtual ~ColliderUI();
};

