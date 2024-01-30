#include "pch.h"
#include "EHParticleSystem.h"

#include "EHDevice.h"
#include "EHStructuredBuffer.h"
#include "EHConstantBuffer.h"

#include "EHTimeMgr.h"
#include "EHAssetMgr.h"
#include "EHMesh.h"
#include "EHMaterial.h"

#include "EHGameObject.h"
#include "EHTransform.h"

ParticleSystem::ParticleSystem()
	:Renderer(RENDERER_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(2000)
{
	SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"ParticleMat"));

	Vec2 vResol = Device::GetInst()->GetResolution();

	m_ParticleBuffer = new StructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, TRUE, STRUCTURED_TYPE::READ_WRITE);

	// 파티클 모듈 정보를 저장하는 구조화버

	m_CSParticleUpdate = (ParticleUpdate*)AssetMgr::GetInst()->FindAsset<ComputeShader>(L"ParticleUpdateShader").Get();

	m_SpawnCountBuffer = new StructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, true, STRUCTURED_TYPE::READ_WRITE);

	// 초기 모듈 셋팅
	m_Module._arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	m_Module._SpaceType = 1;
	m_Module._SpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_Module._SpawnMinScale = Vec4(50.f, 50.f, 1.f, 1.f);
	m_Module._SpawnMaxScale = Vec4(200.f, 200.f, 1.f, 1.f);
	m_Module._MinLife = 0.4f;
	m_Module._MaxLife = 1.f;
	m_Module._SpawnShape = 1;
	m_Module._Radius = 600.f;
	m_Module._SpawnBoxScale = Vec4(900.f, 900.f, 0.f, 0.f);
	m_Module._SpawnRate = 50;

	// Add Velocity Module
	m_Module._arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	m_Module._AddVelocityType = 0;
	m_Module._MinSpeed = 100;
	m_Module._MaxSpeed = 200;
	m_Module._FixedDirection;
	m_Module._FixedAngle;

	m_ParticleModuleBuffer = new StructuredBuffer;
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule), 1, TRUE, STRUCTURED_TYPE::READ_ONLY);
}

ParticleSystem::~ParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	if (nullptr != m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}

void ParticleSystem::UpdateData()
{
}

void ParticleSystem::LateUpdate()
{
	m_Time += DT;
	if ((1.f / m_Module._SpawnRate) < m_Time)
	{
		float fSpawnCount = m_Time / (1.f / m_Module._SpawnRate);

		m_Time -= (1.f / m_Module._SpawnRate) * floorf(fSpawnCount);
		tSpawnCount count = tSpawnCount{ int(fSpawnCount), };
		m_SpawnCountBuffer->SetData(&count);
	}
	else
	{
		tSpawnCount count = tSpawnCount{ 0, };
		m_SpawnCountBuffer->SetData(&count);
	}

	// 중복 바인딩?
	m_ParticleModuleBuffer->SetData(&m_Module);

	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);

	m_CSParticleUpdate->Execute();
	m_CSParticleUpdate->SetParticleWorldPos(Vec3(GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition()));

	// Test
	/*tParticle arrParticle[100] = {};
	m_ParticleBuffer->GetData(arrParticle);

	tParticleModule _module = {};
	m_ParticleModuleBuffer->GetData(&_module);*/
}

void ParticleSystem::Render()
{
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (nullptr == _tr)
		return;

	_tr->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);

	// NoiseTexture Binding
	if (m_NoiseSprite != nullptr)
	{
		m_NoiseSprite->UpdateData(14);
		m_NoiseSprite->UpdateData_CS_SRV(14);
	}

	// GlobalData ComputeShaderBinding
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::GLOBAL)->UpdateData_CS();

	// 파티클 개별 랜더링 -> 인스턴싱
	// 이것도 뭐임?
	//GetMaterial()->SetMaterialParam(INT_0, 0);
	GetMaterial()->SetTexParam(TEX_0, m_ParticleSprite);
	GetMaterial()->UpdateData();
	GetMesh()->Render_Instancing(m_MaxParticleCount);

	m_ParticleBuffer->Clear(20);
}