#pragma once

#include "EHComponentUI.h"

class Light2DUI
	:public ComponentUI
{
private:
	virtual void Render_Update() override;
public:
	Light2DUI();
	virtual ~Light2DUI();
};

