#pragma once

#include "EHUI.h"

class SpriteEditor
	:public UI
{
private:

public:
	virtual void Render_Update() override;

public:
	SpriteEditor();
	virtual ~SpriteEditor();
};

