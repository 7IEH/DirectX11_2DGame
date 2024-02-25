#pragma once
#include "EHComponentUI.h"
class ButtonUI :
    public ComponentUI
{
private:

public:
    virtual void Render_Update()override;

public:
    ButtonUI();
    virtual ~ButtonUI();
};

