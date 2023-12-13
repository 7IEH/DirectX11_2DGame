#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"

MeshRenderer::MeshRenderer()
	:Component(COMPONENT_TYPE::MESHRENDERER)
	,m_Mesh(nullptr)
	,m_Shader(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Tick()
{
	m_Mesh->UpdateData();
	m_Shader->UpdateData();

	m_Mesh->Render();
}

void MeshRenderer::Render()
{
	m_Shader->Render();
}
