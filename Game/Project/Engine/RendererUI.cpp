#include "pch.h"
#include "RendererUI.h"

#include "EHGameObject.h"
#include "EHMeshRenderer.h"

RendererUI::RendererUI()
	:ComponentUI("Renderer","##Renderer",COMPONENT_TYPE::RENDERER)
{
}

RendererUI::~RendererUI()
{
}

void RendererUI::Render_Update()
{
	if(nullptr == GetTargetObject())

	MeshRenderer* _renderer = GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER);
}
