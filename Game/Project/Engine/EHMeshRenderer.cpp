#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"

MeshRenderer::MeshRenderer()
	:Component(COMPONENT_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Tick()
{
	m_Mesh->UpdateData();
	m_Shader->UpdateData();
}

void MeshRenderer::Render()
{
	m_Mesh->Render();
	m_Shader->Render();
}
