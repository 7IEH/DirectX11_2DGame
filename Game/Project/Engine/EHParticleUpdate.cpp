#include "pch.h"
#include "EHParticleUpdate.h"

ParticleUpdate::ParticleUpdate()
	:ComputeShader(1024,1,1)
	,m_ParticleBuffer(nullptr)
{
	Create(L"\\shader\\particle_update.fx", "CS_ParticleUpdate");
}

ParticleUpdate::~ParticleUpdate()
{
}


HRESULT ParticleUpdate::UpdateData()
{
	if (nullptr == m_ParticleBuffer)
		return E_FAIL;

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_ParticleBuffer->UpdateData_CS_UAV(0);
	m_SpawnCountBuffer->UpdateData_CS_UAV(1);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	UpdateGroupCount();

	return S_OK;
}

void ParticleUpdate::UpdateGroupCount()
{
	UINT GroupX = (m_ParticleBuffer->GetElementCount() / m_ThreadX);
	if (0 != m_ParticleBuffer->GetElementCount() % m_ThreadX)
		GroupX += 1;

	SetGroupX(GroupX); SetGroupY(1); SetGroupZ(1);
}

void ParticleUpdate::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_ParticleModuleBuffer->Clear_CS_SRV();
	m_ParticleModuleBuffer = nullptr;

	m_SpawnCountBuffer->Clear_CS_UAV();
	m_SpawnCountBuffer = nullptr;
}