#include "pch.h"
#include "EHRenderer.h"

Renderer::Renderer(RENDERER_TYPE _type)
	:Component(COMPONENT_TYPE::RENDERER)
	,m_Type(_type)
	,m_Mesh(nullptr)
	,m_Shader(nullptr)
{
}

Renderer::~Renderer()
{
	if (m_Mesh != nullptr)
	{
		delete m_Mesh;
		m_Mesh = nullptr;
	}

	if (m_Shader != nullptr)
	{
		delete m_Shader;
		m_Shader = nullptr;
	}
}
