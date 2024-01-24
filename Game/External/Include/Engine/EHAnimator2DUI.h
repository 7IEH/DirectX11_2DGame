#pragma once
#include "EHComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
private:
    map<string, bool> m_AnimationInfo;
    string            m_prevAnimation2D;

public:
    virtual void Render_Update()override;
public:
    Animator2DUI();
    virtual ~Animator2DUI();
};

