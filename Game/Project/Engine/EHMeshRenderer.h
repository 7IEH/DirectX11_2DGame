#pragma once

#include "EHComponent.h"
class Mesh;
class Shader;
class MeshRenderer
	:public Component
{
private:
	Mesh*				m_Mesh;
	Shader*				m_Shader;

public:
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	// Access Func
	void SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
	Mesh* GetMesh() { return m_Mesh; }

	void SetShader(Shader* _shader) { m_Shader = _shader; }
	Shader* GetShader() { return m_Shader; }

public:
	virtual void Create() override {};
	virtual void Tick() override;
	virtual void Render() override;
};

