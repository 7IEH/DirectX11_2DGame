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
	virtual void UpdateData() override;
	virtual void FinalTick() override {};
	virtual void Render() override;
};