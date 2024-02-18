#include "pch.h"
#include "EHRenderer.h"

Renderer::Renderer(RENDERER_TYPE _type)
	:Component(COMPONENT_TYPE::RENDERER)
	, m_Type(_type)
	, m_Mesh(nullptr)
	, m_Material(nullptr)
{
}

Renderer::Renderer(const Renderer& _origin)
	:Component(_origin)
	, m_Type(_origin.m_Type)
	, m_Mesh(_origin.m_Mesh)
	, m_Material(_origin.m_Material)
{
	// 동적 재질
}

Renderer::~Renderer()
{
}