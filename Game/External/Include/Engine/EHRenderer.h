#pragma once

#include "EHComponent.h"

#include "EHMesh.h"
#include "EHMaterial.h"

class Renderer
	:public Component
{
private:
	RENDERER_TYPE			m_Type;
	Ptr<Mesh>				m_Mesh;
	Ptr<Material>			m_Material;

public:
	RENDERER_TYPE			GetRenderType() { return m_Type; }
	Ptr<Mesh>				GetMesh() { return m_Mesh; }
	Ptr<Material>			GetMaterial() { return m_Material; }

	void				SetMesh(Ptr<Mesh> _mesh) { m_Mesh = _mesh; }
	void				SetMaterial(Ptr<Material> _material) { m_Material = _material; }

public:
	virtual void Render() = 0;
	virtual void Save(string _path)override {};

public:
	Renderer(RENDERER_TYPE _type);
	~Renderer();
};

