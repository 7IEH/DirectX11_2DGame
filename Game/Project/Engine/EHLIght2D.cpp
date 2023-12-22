#include "pch.h"
#include "EHLIght2D.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

LIght2D::LIght2D()
	:Component(COMPONENT_TYPE::LIGHT2D)
	,m_PL(nullptr)
	,m_SL(nullptr)
{
	m_DL = new DirectinalLight;
	m_DL->Ambient = Vec4(0.0f, 0.0f, 0.1f, 1.0f);
	m_DL->Diffuse = Vec4(0.0f, 0.0f, 0.1f, 1.0f);
	m_DL->Specular = Vec4(0.0f, 0.0f, 0.1f, 1.0f);
	m_DL->Direction = Vec3(0.f, 0.f, 0.57735f);
}

LIght2D::~LIght2D()
{
}

void LIght2D::FinalTick()
{
	// Light 값 셋팅
	// 근데 2D는 global illumination Light setting 해줘야함 ㅋㅋ;
	// DirectionLight
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::LIGHT)->SetData(m_DL, sizeof(DirectinalLight), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::LIGHT)->UpdateData();
}
