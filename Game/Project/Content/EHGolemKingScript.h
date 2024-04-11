#pragma once
#include "EHEnemyScript.h"

enum class BossState
{
	Idle,
	Attack,
	Pause,
	Dead,
};

enum class Pattern
{
	LaunchArm,
	SpawnRocks,
	Sticky,
	Wave,
};

class GolemKingScript :
	public EnemyScript
{
private:
	/****************
	| Basic Param
	****************/
	BossState m_eState;
	bool	m_bArm;

	/****************
	| Attack Param
	****************/
	Pattern m_ePattern;
	float	m_fAttackTime;
	float	m_fAcctime;
	bool	m_fWaitingTime1;
	GameObject* m_pPlayer;

	/***************
	| Launch Arm
	***************/
	GameObject* m_pArmObject1;
	int	 m_bAttackCount;
	bool m_bTrackCompleted;

	/***************
	| SpawnRocks
	***************/
	bool m_bSpawnRocks;
	
	/************
	| Wave Attack
	************/
	GameObject* m_pWaveObject;

public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdtae();

public:
	virtual void OnTriggerEnter(Collider* _other)override;

public:
	void SetState(BossState _eState) { m_eState = _eState; }
	BossState Getstate() { return m_eState; }

	void SetPattern(Pattern _ePattern) { m_ePattern = _ePattern; }

private:
	void Idle();
	void Attack();
	void Dead();
	void Pause();

private:
	void LaunchArm();
	void SpawnRocks();
	void Sticky();
	void Wave();

	void SpawnShadow(Vec2 _vPos);
public:
	CLONE(GolemKingScript)
	GolemKingScript();
	virtual ~GolemKingScript();
};

