#pragma once

#include "EHUI.h"

class GameView
	:public UI
{
private:

public:
	virtual void Render_Update() override;

public:
	GameView();
	virtual ~GameView();
};

