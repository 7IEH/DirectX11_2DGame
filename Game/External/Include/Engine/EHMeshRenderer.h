#pragma once

#include "EHRenderer.h"
class Mesh;
class Shader;
class MeshRenderer
	:public Renderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	virtual void Tick() override;
	virtual void Render() override;
};

