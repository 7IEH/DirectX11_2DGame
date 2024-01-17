#pragma once
#include "EHComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
public:
    virtual void Render_Update()override;
public:
    Animator2DUI();
    virtual ~Animator2DUI();
};

