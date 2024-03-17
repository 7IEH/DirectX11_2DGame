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

#include "EHAssetMgr.h"

MeshRenderer::MeshRenderer()
	:Renderer(RENDERER_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::UpdateData()
{
	Ptr<Material> _mat = GetMaterial();
	GameObject* _owner = GetOwner();

	// Matrial Update
	if (_mat.Get() != nullptr)
	{
		if (_owner->GetPicking())
		{
			_mat.Get()->SetMaterialParam(INT_3, 1);
		}
		else
		{
			_mat.Get()->SetMaterialParam(INT_3, 0);
		}
		_mat.Get()->Render();
	}

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

void MeshRenderer::Save(std::wofstream* _file)
{
	// 1. Mesh 2. Material
	*_file << L"MESHRENDERER\n";

	if (nullptr != GetMesh())
	{
		*_file << GetMesh()->GetName() + L'\n';
	}

	if (nullptr != GetMaterial())
	{
		*_file << GetMaterial()->GetName() + L'\n';
	}
}

void MeshRenderer::Load(std::wifstream* _file)
{
	wstring _line = L"";

	for (int i = 0;i < 2;i++)
	{
		std::getline(*_file, _line);
		if (i == 0)
		{
			SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(_line));
		}
		else
		{
			Ptr<Material> _mtrl = AssetMgr::GetInst()->FindAsset<Material>(_line);
			SetMaterial(_mtrl->Clone());
		}
	}
}
