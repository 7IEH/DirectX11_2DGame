#pragma once
#include "EHUI.h"
class TilePalette :
    public UI
{
private:

public:
    virtual void Render_Update()override;

public:
    TilePalette();
    virtual ~TilePalette();
};

