#pragma once
#include "EHRenderer.h"
class CanvasRenderer :
	public Renderer
{
private:

public:
	virtual void LateUpdate()override {};
	virtual void Render()override;

	CLONE(CanvasRenderer)
public:
	CanvasRenderer();
	virtual ~CanvasRenderer();

	virtual void Save(string _path)override;
};

