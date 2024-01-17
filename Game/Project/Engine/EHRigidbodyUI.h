#pragma once
#include "EHComponentUI.h"

class RigidbodyUI
	:public ComponentUI
{
public:
	virtual void Render_Update()override;

public:
	RigidbodyUI();
	virtual ~RigidbodyUI();
};

