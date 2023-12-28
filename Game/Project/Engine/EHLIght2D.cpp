#include "pch.h"
#include "EHLIght2D.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

LIght2D::LIght2D()
	:Component(COMPONENT_TYPE::LIGHT2D)
{
	/*m_PL.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_PL.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PL.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PL.Att = Vec3(0.f, 0.1f, 0.f);
	m_PL.Range = 500.f;
	m_PL.Position = Vec3(0.f, 450.f, -10.f);*/

	m_SL.Ambient = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	m_SL.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SL.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SL.Att = Vec3(1.f, 0.0f, 0.f);
	m_SL.Spot = 0.5f;
	m_SL.Range = 1000.f;
	m_SL.Position = Vec3(0.f, 450.f, 500.f);
	m_SL.Direction = Vec3(0.f, 0.2f, -0.5f);
}

LIght2D::~LIght2D()
{
}

void LIght2D::LateUpdate()
{
	// Light 값 셋팅
	// 근데 2D는 global illumination Light setting 해줘야함 ㅋㅋ;
	// DirectionLight
	m_Light._DL = m_DL;
	m_Light._PL = m_PL;
	m_Light._SL = m_SL;

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::LIGHT)->SetData(&m_Light, sizeof(tLight), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::LIGHT)->UpdateData();
}
