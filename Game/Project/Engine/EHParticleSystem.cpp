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
	, m_MaxParticleCount(1000)
{
	SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"PointMesh"));
	SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"ParticleMat"));

	Vec2 vResol = Device::GetInst()->GetResolution();

	m_CSParticleUpdate = (ParticleUpdate*)AssetMgr::GetInst()->FindAsset<ComputeShader>(L"ParticleUpdateShader").Get();

	m_SpawnCountBuffer = new StructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, true, STRUCTURED_TYPE::READ_WRITE);

	m_ParticleBuffer = new StructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, true, STRUCTURED_TYPE::READ_WRITE);

	// ��ƼŬ ��� ������ �����ϴ� ����ȭ��
	m_ParticleModuleBuffer = new StructuredBuffer;
	/*UINT ModuleAddSize = 0;
	if (sizeof(tParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tParticleModule) % 16);
	}*/
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule), 1, TRUE, STRUCTURED_TYPE::READ_ONLY);
}

ParticleSystem::ParticleSystem(const ParticleSystem& _origin)
	:Renderer(_origin)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(_origin.m_MaxParticleCount)
	, m_Module(_origin.m_Module)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_CSParticleUpdate(_origin.m_CSParticleUpdate)
	, m_ParticleSprite(_origin.m_ParticleSprite)
	, m_NoiseSprite(_origin.m_NoiseSprite)
	, m_Time(0.f)
{
	if (nullptr != _origin.m_ParticleBuffer)
		m_ParticleBuffer = _origin.m_ParticleBuffer->Clone();

	if (nullptr != _origin.m_ParticleModuleBuffer)
		m_ParticleModuleBuffer = _origin.m_ParticleModuleBuffer->Clone();

	if (nullptr != _origin.m_SpawnCountBuffer)
		m_SpawnCountBuffer = _origin.m_SpawnCountBuffer->Clone();
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

	m_ParticleModuleBuffer->SetData(&m_Module);

	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);

	m_CSParticleUpdate->Execute();
	m_CSParticleUpdate->SetParticleWorldPos(Vec3(GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition()));

	// Test
	tParticle arrParticle[14] = {};
	m_ParticleBuffer->GetData(arrParticle, 14);

	tParticleModule _module = {};
	m_ParticleModuleBuffer->GetData(&_module);
}

void ParticleSystem::Render()
{
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	if (nullptr == _tr)
		return;

	_tr->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);

	// NoiseTexture Binding
	if (m_NoiseSprite != nullptr)
	{
		m_NoiseSprite->UpdateData(14);
		m_NoiseSprite->UpdateData_CS_SRV(14);
	}

	// GlobalData ComputeShaderBinding
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::GLOBAL)->UpdateData_CS();

	// ��ƼŬ ���� ������ -> �ν��Ͻ�
	// �̰͵� ����?
	//GetMaterial()->SetMaterialParam(INT_0, 0);
	GetMaterial()->SetTexParam(TEX_0, m_ParticleSprite);
	GetMaterial()->UpdateData();
	GetMesh()->Render_Instancing(m_MaxParticleCount);

	m_ParticleBuffer->Clear(20);
}

void ParticleSystem::Save(string _path)
{
	std::ofstream _file(_path.data(), std::fstream::out | std::fstream::app);

	_file << "PARTICLESYSTEM\n";

	// Module
	string _line;
	_file << EH::ConvertString(m_NoiseSprite.Get()->GetName()) + '\n';
	_file << EH::ConvertString(m_ParticleSprite.Get()->GetName()) + '\n';

	_file << EH::WriteVector4(m_Module._SpawnColor) + '\n';
	_file << EH::WriteVector4(m_Module._SpawnMinScale) + '\n';
	_file << EH::WriteVector4(m_Module._SpawnMaxScale) + '\n';

	_file << std::to_string(m_Module._iLoop) + '\n';
	_file << std::to_string(m_Module._MinLife) + '\n';
	_file << std::to_string(m_Module._MaxLife) + '\n';
	_file << std::to_string(m_Module._MinMass) + '\n';
	_file << std::to_string(m_Module._MaxMass) + '\n';
	_file << std::to_string(m_Module._SpawnRate) + '\n';
	_file << std::to_string(m_Module._SpaceType) + '\n';
	_file << std::to_string(m_Module._SpawnShape) + '\n';
	_file << std::to_string(m_Module._Radius) + '\n';
	_file << EH::WriteVector4(m_Module._SpawnBoxScale) + '\n';
	_file << EH::WriteVector4(m_Module._ScaleRatio) + '\n';
	_file << std::to_string(m_Module.NoiseForceScale) + '\n';
	_file << std::to_string(m_Module.NoiseForceTerm) + '\n';
	_file << std::to_string(m_Module._AddVelocityType) + '\n';
	_file << std::to_string(m_Module._MinSpeed) + '\n';
	_file << std::to_string(m_Module._MaxSpeed) + '\n';
	_file << std::to_string(m_Module._FixedAngle) + '\n';
	_file << EH::WriteVector4(m_Module._FixedDirection) + '\n';

	_file << std::to_string(m_Module._ColorType);

	for (UINT i = 0;i < (UINT)PARTICLE_MODULE::END;i++)
	{
		_file << std::to_string(m_Module._arrModuleCheck[i]) + '\n';
	}

	// Buffer
	tParticle _paricleBuffer[1000];
	UINT	_particleCount = m_ParticleBuffer->GetCurElementCount();
	m_ParticleBuffer->GetData(_paricleBuffer, _particleCount);

	_file << _particleCount + '\n';

	for (UINT i = 0;i < _particleCount;i++)
	{
		_file << EH::WriteVector4(_paricleBuffer[i]._LocalPos) + '\n';
		_file << std::to_string(_paricleBuffer[i]._iFadeVariable) + '\n';
	}
}
