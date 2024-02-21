#include "pch.h"
#include "EHMaterial.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHGraphicShader.h"

#include "EHSprite.h"

Material::Material()
	:Asset(ASSET_TYPE::MATERIAL)
	,m_tMaterial{}
{
	m_tMaterial.vLightMat.Color = Vec4(1.f, 1.f, 1.f, 1.f);
}

Material::~Material()
{
}

void Material::SetTexParam(TEX_PARAM _Param, Ptr<Sprite> _sprite)
{
	m_Sprite[_Param] = _sprite;
}

void Material::UpdateData()
{
	if (GetGraphicShader().Get() == nullptr)
	{
		HandleError(MAIN_HWND, L"MeshRenderShader Shader is Nullptr Error!", 2);
		return;
	}
	GetGraphicShader()->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr != m_Sprite[i].Get())
		{
			m_Sprite[i]->UpdateData(i);
			m_tMaterial.Sprite[i] = 1;
		}
		else
		{
			Sprite::Clear(i);
			m_tMaterial.Sprite[i] = 0;
		}
	}

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->SetData(&m_tMaterial,sizeof(material),1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->UpdateData();
}

void Material::Render()
{
	UpdateData();
}