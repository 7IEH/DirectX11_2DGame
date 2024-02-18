#pragma once

#include "EHComponent.h"

class Collider
	:public Component
{
private:
	UINT				m_CollisionCount;
	COLLIDER_TYPE		m_Type;

	bool				m_Enabled;
	bool				m_Absolute;

public:
	UINT GetCollisionCount() { return m_CollisionCount; }
	void SetCollisionCount(UINT _collisionCount) { m_CollisionCount = _collisionCount; }

	COLLIDER_TYPE GetColliderType() { return m_Type; }

	void SetAbsolute(bool _absolute) { m_Absolute = _absolute; }
	bool GetAbsolute() { return m_Absolute; }

	bool	GetEnabled() { return m_Enabled; }
	void	Enabled() { m_Enabled = !m_Enabled; }

public:
	virtual void DrawCollider() = 0;
	virtual void LateUpdate()override {};

	virtual void Save(string _path)override {};

	virtual Collider* Clone() = 0;

private:
	virtual void OnTriggerEnter(Collider* _other);
	virtual void OnTriggerStay(Collider* _other);
	virtual void OnTriggerExit(Collider* _other);

public:
	Collider(COLLIDER_TYPE _type);
	Collider(const Collider& _origin);
	virtual ~Collider();

	friend class CollisionMgr;
};

