#pragma once

#include "EHComponentUI.h"

class TileMapRenderer
	:public ComponentUI
{
private:

public:
	virtual void Render_Update()override;

public:
	TileMapRenderer();
	virtual ~TileMapRenderer();
};

