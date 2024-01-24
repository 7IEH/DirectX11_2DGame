#pragma once

#include "EHColliderUI.h"

class BoxCollider2DUI
	:public ColliderUI
{
private:

public:
	virtual void Render_Update()override;

public:
	BoxCollider2DUI();
	virtual ~BoxCollider2DUI();
};

