#pragma once

#include "EHComponentUI.h"
 
class TileMapCollider
	:public ComponentUI
{
private:

public:
	virtual void Render_Update() override;

public:
	TileMapCollider();
	virtual ~TileMapCollider();
};

