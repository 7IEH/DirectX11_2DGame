#include "pch.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHShader.h"
#include "EHMaterial.h"
#include "EHSprite.h"

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
		e_MatrixData.World = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetMatWorld();
		e_MatrixData.WorldInv = XMMatrixInverse(nullptr,e_MatrixData.World);
		
		e_MatrixData.ViewInv = XMMatrixInverse(nullptr, e_MatrixData.View);

		e_MatrixData.ProjInv = XMMatrixInverse(nullptr, e_MatrixData.Projection);

		XMMATRIX _worldtemp = XMMatrixTranspose(e_MatrixData.World);
		XMMATRIX _viewtemp = XMMatrixTranspose(e_MatrixData.View);
		XMMATRIX _projectiontemp = XMMatrixTranspose(e_MatrixData.Projection);

		// WV
		e_MatrixData.WV = XMMatrixMultiply(_worldtemp, _viewtemp);

		// WVP
		e_MatrixData.WVP = XMMatrixTranspose(XMMatrixMultiply(e_MatrixData.WV, _projectiontemp));

		e_MatrixData.WV = XMMatrixTranspose(e_MatrixData.WV);

		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->SetData(&e_MatrixData, sizeof(transform), 1);
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData();

		NomralVector* temp = new NomralVector();
		temp->Nomral = _tr->GetWorldDir(DIRECTION_TYPE::FRONT);

		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::NORMANL)->SetData(&temp, sizeof(NomralVector), 1);
		Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::NORMANL)->UpdateData();
	}

	UpdateData();

	// Matrial Update
	if(GetMaterial() != nullptr)
		GetMaterial()->Render();

	// Sprite Update
	if(GetSprite() != nullptr)
		GetSprite()->UpdateData(0);


	GetShader()->Render();
	GetMesh()->Render();
}
