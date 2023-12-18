#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"

#include "EHDevice.h"

#include "EHConstantBuffer.h"
#include "EHGameObject.h"
#include "EHTransform.h"
#include "EHCamera.h"


extern Camera* MainCamera;

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
		transform _tempData;
		_tempData._world = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();
		_tempData._view = MainCamera->GetMatView();
		_tempData._projection = MainCamera->GetMatProj();

		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->SetData(&_tempData,sizeof(transform), 1);
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData();
	}
	GetMesh()->UpdateData();
	GetShader()->Render();
	GetMesh()->Render();
}
