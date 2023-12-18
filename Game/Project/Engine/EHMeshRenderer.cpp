#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"

#include "EHDevice.h"

#include "EHConstantBuffer.h"
#include "EHGameObject.h"
#include "EHTransform.h"
#include "EHCamera.h"

extern transform e_MatrixData;

MeshRenderer::MeshRenderer()
	:Renderer(RENDERER_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::UpdateData()
{
}

void MeshRenderer::FinalTick()
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
		e_MatrixData._world = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();

		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->SetData(&e_MatrixData,sizeof(transform), 1);
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData();
	}
	GetShader()->Render();
	GetMesh()->Render();
}
