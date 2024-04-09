#pragma once

#include <EHScript.h>

enum class EnemyState
{
	Idle,
	Attack,
	Chase,
	Dead,
	None,
};

enum class EnemyType
{
	Melee,
	Ranged,
	None,
};

enum class EnemyDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class EnemyScript
	:public Script
{
private:
	bool	   m_eChase;
	EnemyState m_eState;
	EnemyType  m_eEnemyType;
	EnemyDir   m_eDir;

	int			m_iHp;

	int			m_iDamage;

	float		m_fChargeTime;
	float		m_fAccTime;

public:
	virtual void Awake()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void LateUpdate()override;

	virtual void OnTriggerEnter(Collider* _other)override;

public:
	void SetType(EnemyType _eType) { m_eEnemyType = _eType; }
	void SetState(EnemyState _eState) { m_eState = _eState; }

	void SetHp(UINT _iHp) { m_iHp = _iHp; }

	void SetDamage(UINT _iDamage) { m_iDamage = _iDamage; }
	UINT GetDamage() { return m_iDamage; }

	void SetChase(bool _bChase) { m_eChase = _bChase; }
	bool GetChase() { return m_eChase; }

	void SetDir(EnemyDir _eDir) { m_eDir = _eDir; }
	EnemyDir GetDir() { return m_eDir; }

	void SetChargeTime(float _fChargeTime) { m_fChargeTime = _fChargeTime; }
	void SetAccTime(float _fAccTime) { m_fAccTime = _fAccTime; }

	float GetChargeTime() { return m_fChargeTime; }
	float GetAccTime() { return m_fAccTime; }

protected:
	void CaculateDir();

public:
	virtual void Idle() {};
	virtual void Chase() {};
	virtual void Attack() {};
	void Dead();

public:
	CLONE(EnemyScript)
		EnemyScript();
	virtual ~EnemyScript();
};

