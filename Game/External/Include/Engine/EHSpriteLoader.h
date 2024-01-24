#pragma once
#include "EHUI.h"
class SpriteLoader :
    public UI
{
private:
    string m_Name;
    string m_Address;

public:
    virtual void Render_Update()override;
public:
    SpriteLoader();
    virtual ~SpriteLoader();
};

