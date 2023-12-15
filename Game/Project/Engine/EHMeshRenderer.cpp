#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"

#include "EHDevice.h"

#include "EHConstantBuffer.h"
#include "EHGameObject.h"
#include "EHTransform.h"

MeshRenderer::MeshRenderer()
	:Renderer(RENDERER_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Tick()
{
	if (GetShader() == nullptr)
	{
		HandleError(MAIN_HWND, L"MeshRenderShader Shader is Nullptr Error!", 2);
		return;
	}
	
	GetShader()->UpdateData();
}

void MeshRenderer::Render()
{
	if (GetMesh() == nullptr)
	{
		HandleError(MAIN_HWND, L"MeshRenderer Mesh is Nullptr Error!", 2);
		return;
	}
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_tr != nullptr)
	{
		transform* _tData = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatrix();
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->SetData(_tData,sizeof(transform), 1);
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData();
	}
	GetMesh()->UpdateData();
	GetShader()->Render();
	GetMesh()->Render();
}
