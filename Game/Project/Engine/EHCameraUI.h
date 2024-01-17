#pragma once
#include "EHComponentUI.h"
class CameraUI :
    public ComponentUI
{
public:
	virtual void Render_Update()override;

public:
	CameraUI();
	virtual ~CameraUI();
};

