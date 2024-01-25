#include "pch.h"
#include "EHParticleSystem.h"

#include "EHDevice.h"
#include "EHStructuredBuffer.h"

#include "EHAssetMgr.h"
#include "EHMesh.h"
#include "EHMaterial.h"

#include "EHGameObject.h"
#include "EHTransform.h"

ParticleSystem::ParticleSystem()
	:Renderer(RENDERER_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(5)
{
	SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"ParticleMat"));

	Vec2 vResol = Device::GetInst()->GetResolution();

	tParticle arrParticle[5] = {};


	for (UINT i = 0;i < m_MaxParticleCount;i++)
	{
		arrParticle[i]._WorldPos = Vec4((vResol.x / -2.f) + (i + 1) * vResol.x / (m_MaxParticleCount + 1), 0.f, 200.f, 1.f);
		arrParticle[i]._WorldScale = Vec4(50.f, 50.f, 1.f, 1.f);
	}

	m_ParticleBuffer = new StructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), 5, TRUE, STRUCTURED_TYPE::READ_WRITE, arrParticle);
}

ParticleSystem::~ParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}

void ParticleSystem::UpdateData()
{
}

void ParticleSystem::LateUpdate()
{


}

void ParticleSystem::Render()
{
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (nullptr == _tr)
		return;

	_tr->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);

	// 모든 파티클 렌더링
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		GetMaterial()->SetMaterialParam(INT_0, i);
		GetMaterial()->UpdateData();
		GetMesh()->Render();
	}
}