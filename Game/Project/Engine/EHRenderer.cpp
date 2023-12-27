#include "pch.h"
#include "EHRenderer.h"

Renderer::Renderer(RENDERER_TYPE _type)
	:Component(COMPONENT_TYPE::RENDERER)
	,m_Type(_type)
	,m_Mesh(nullptr)
	,m_Material(nullptr)
{
}

Renderer::~Renderer()
{
}
