#include "pch.h"
#include "EHMaterial.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

Material::Material()
	:Asset(ASSET_TYPE::MATERIAL)
	,m_tMaterial(nullptr)
{
	m_tMaterial = new material();
}

Material::~Material()
{
}

void Material::UpdateData()
{
	m_tMaterial->Ambient = Vec4(0.48f, 0.77f, 0.46f, 1.0f);
	m_tMaterial->Diffuse = Vec4(0.48f, 0.77f, 0.46f, 1.0f);
	m_tMaterial->Specular = Vec4(0.2f, 0.2f, 0.2f, 16.0f);

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->SetData(m_tMaterial,sizeof(material),1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->UpdateData();
}

void Material::Render()
{
	UpdateData();
}