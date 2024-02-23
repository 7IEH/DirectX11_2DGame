#pragma once
#include "EHComponent.h"

class Text :
    public Component
{
private:
    wstring m_wTextInput;
    void*   m_pFont;


public:
    virtual void Update();
    virtual void Render();

public:
    Text();
    virtual ~Text();
};

