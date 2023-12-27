#include "pch.h"
#include "EHMaterial.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHGraphicShader.h"

Material::Material()
	:Asset(ASSET_TYPE::MATERIAL)
	,m_tMaterial(nullptr)
{
	m_tMaterial = new material();
}

Material::~Material()
{
	delete m_tMaterial;
}

void Material::UpdateData()
{
	if (GetGraphicShader() == nullptr)
	{
		HandleError(MAIN_HWND, L"MeshRenderShader Shader is Nullptr Error!", 2);
		return;
	}
	GetGraphicShader()->UpdateData();

	
	m_tMaterial->vLightMat.Ambient = Vec4(0.48f, 0.77f, 0.46f, 1.0f);
	m_tMaterial->vLightMat.Diffuse = Vec4(1.f, 1.f, 1.f, 1.0f);
	m_tMaterial->vLightMat.Specular = Vec4(0.2f, 0.2f, 0.2f, 16.0f);

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->SetData(m_tMaterial,sizeof(material),1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->UpdateData();
}

void Material::Render()
{
	UpdateData();

	GetGraphicShader()->Render();
}