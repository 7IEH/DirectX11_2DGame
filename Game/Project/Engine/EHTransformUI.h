#pragma once
#include "EHComponentUI.h"

class TransformUI :
    public ComponentUI
{
public:
    virtual void Render_Update()override;
public:
    TransformUI();
    virtual ~TransformUI();
};

