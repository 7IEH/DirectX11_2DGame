#include "pch.h"
#include "EHRigidBody.h"

#include "EHTimeMgr.h"
#include "EHGameObject.h"

RigidBody::RigidBody()
	:Component(COMPONENT_TYPE::RIGIDBODY)
	, m_fMass(1.f)
	, m_fFriction(100.f)
	, m_fCoefficient(0.f)
	, m_vVelocity{}
	, m_vAccelation{}
	, m_vForce{}
	, m_vGravity{}
	, m_vMaxGravity{}
{
	m_vGravity = Vec2(0.f, 800.f);
	m_vMaxGravity = Vec2(200.f, 1000.f);
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
	m_vAccelation = m_vForce / m_fMass;

	m_vVelocity += Vec2(m_vAccelation.x * DT, m_vAccelation.y * DT);

	if (m_vVelocity != Vec2(0.f, 0.f))
	{
		Vec2 _vFriction = -m_vVelocity;
		_vFriction.Normalize();
		_vFriction.x = _vFriction.x	* m_fFriction * m_fMass * DT;
		_vFriction.y = _vFriction.y	* m_fFriction * m_fMass * DT;

		if (m_vVelocity.Length() < _vFriction.Length())
		{
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += _vFriction;
		}
	}

	Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vPos = _pTr->GetRelativePosition();
	if (nullptr != _pTr)
	{
		_vPos.x = _vPos.x + m_vVelocity.x * DT;
		_vPos.y = _vPos.y + m_vVelocity.y * DT;
		_pTr->SetRelativePosition(_vPos);
	}

	Clear();
}

void RigidBody::LateUpdate()
{
}

void RigidBody::UpdateData()
{
}

void RigidBody::Save(std::wofstream* _file)
{
}

void RigidBody::Load(std::wifstream* _file)
{
}