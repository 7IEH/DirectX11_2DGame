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
#include "EHAnimator2D.h"

MeshRenderer::MeshRenderer()
	:Renderer(RENDERER_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::UpdateData()
{
	// Matrial Update
	if (GetMaterial() != nullptr)
		GetMaterial()->Render();

	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (_tr != nullptr)
	{
		_tr->UpdateData();
	}
}

void MeshRenderer::Render()
{
	Animator2D* _animator = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	if (_animator != nullptr)
	{
		_animator->UpdateData();
	}
	else
	{
		_animator->Clear();
	}

	UpdateData();

	if (GetMesh() == nullptr)
	{
		HandleError(MAIN_HWND, L"MeshRenderer Mesh is Nullptr Error!", 2);
		return;
	}
	GetMesh()->Render();
}
