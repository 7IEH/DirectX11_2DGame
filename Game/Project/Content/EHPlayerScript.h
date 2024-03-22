#pragma once
#include "EHScript.h"
#include "EHRecordManager.h"

enum class	State
{
	Idle,
	Attack,
	Move,
	Dodge,
	Dead,
	None,
};

enum class Dir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	None,
};

class PlayerScript
	:public Script
{
private:
	Dir					m_eDir;
	State				m_eState;
	PlayerPref*			m_pPlayerPref;

	float				m_fAccTime;
	// ���� ���� �ð�
	Vec4				m_vPrevPos;
	float				m_fClifTime;
	bool				m_bFalling;

	// ���� ���� �ð�
	float				m_fInvincibilityTime;

	// Trigger ���� �ð�
	float				m_fTriggerTime;

	bool				m_bInvincibility;
	vector<bool>		m_vEnterWall_Move;
	vector<bool>		m_vEnterWall_Dodge;

public:
	virtual void Awake()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void LateUpdate() override {};

	virtual void OnTriggerEnter(Collider* _other);
	virtual void OnTriggerStay(Collider* _other);
	virtual void OnTriggerExit(Collider* _other);

	CLONE(PlayerScript)

private:
	void Invincibility();
	void Idle();
	void Attack();
	void Move();
	void Dodge();
	void Dead();

	void IdleAnim();
	void AttackAnim();
	void MoveAnim();
	void DodgeAnim();
	void FallingAnim();
	void DeadAnim();
public:
	PlayerScript();
	virtual ~PlayerScript();
};

