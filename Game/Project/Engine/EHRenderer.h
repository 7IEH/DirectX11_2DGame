#pragma once

#include "EHComponent.h"
class Mesh;
class Shader;
class Material;
class Sprite;

class Renderer
	:public Component
{
private:
	RENDERER_TYPE		m_Type;
	Mesh*				m_Mesh;
	Material*			m_Material;
	Sprite*				m_Sprite;

public:
	RENDERER_TYPE		GetRenderType() { return m_Type; }
	Mesh*				GetMesh() { return m_Mesh; }
	Sprite*				GetSprite()	{return m_Sprite;}
	Material*			GetMaterial() { return m_Material; }


	void				SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
	void				SetMaterial(Material* _material) { m_Material = _material; }
	void				SetSprite(Sprite* _sprite) { m_Sprite = _sprite; }

public:
	virtual void Render() = 0;

public:
	Renderer(RENDERER_TYPE _type);
	~Renderer();
};

