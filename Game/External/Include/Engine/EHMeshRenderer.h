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
	virtual void LateUpdate() override {};
	virtual void Render() override;

	virtual void Save(string _path) override;
	virtual void Load(std::wifstream* _file) override;

	CLONE(MeshRenderer)
};