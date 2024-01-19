#pragma once

#include "EHUI.h"

class SceneView
	:public UI
{
private:

public:
	virtual void Render_Update() override;

public:
	SceneView();
	virtual ~SceneView();
};

