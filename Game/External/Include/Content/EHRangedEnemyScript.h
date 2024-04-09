#pragma once
#include "EHEnemyScript.h"

class RangedEnemyScript :
	public EnemyScript
{
public:
	virtual void Update() override;

public:
	virtual void Idle()override;
	virtual void Chase()override;
	virtual void Attack()override;

protected:
	void LaunchMissile(const wstring& _sPlayName,const wstring& _sDeadName,Dir _eDir,float _fSPeed,float _fDamage,Vec4 _vPos,Vec4 _vScale);

public:
	RangedEnemyScript();
	virtual ~RangedEnemyScript();
};
