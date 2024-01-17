#pragma once
#include "EHComponentUI.h"
class RendererUI :
    public ComponentUI
{
public:
    virtual void Render_Update();
public:
    RendererUI();
    virtual ~RendererUI();
};

