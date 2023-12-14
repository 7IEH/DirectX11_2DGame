#pragma once

#include "EHComponent.h"
class Mesh;
class Shader;

class Renderer
	:public Component
{
private:
	RENDERER_TYPE		m_Type;
	Mesh*				m_Mesh;
	Shader*				m_Shader;

public:
	RENDERER_TYPE		GetRenderType() { return m_Type; }
	Mesh*				GetMesh() { return m_Mesh; }
	Shader*				GetShader() { return m_Shader; }

	void				SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
	void				SetShader(Shader* _shader) { m_Shader = _shader; }
	
public:
	virtual void Render() = 0;

public:
	Renderer(RENDERER_TYPE _type);
	~Renderer();
};

