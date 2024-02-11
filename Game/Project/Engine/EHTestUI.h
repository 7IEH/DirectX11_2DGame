#pragma once
#include "EHUI.h"

class TestUI :
    public UI
{
private:
    float m_Width;

public:
    virtual void Render_Update();

public:
    TestUI();
    virtual ~TestUI();
};

