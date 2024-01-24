#pragma once

#include "EHColliderUI.h"

class CircleCollider2DUI
	:public ColliderUI
{
private:

public:
	virtual void Render_Update()override;

public:
	CircleCollider2DUI();
	virtual ~CircleCollider2DUI();
};