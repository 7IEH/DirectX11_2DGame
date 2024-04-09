#pragma once

#include "EHEnemyScript.h"

class MeleeEnemyScript :
	public EnemyScript
{
private:

public:
	virtual void Update() override;

public:
	virtual void Idle()override;
	virtual void Chase()override;
	virtual void Attack()override;

public:
	MeleeEnemyScript();
	virtual ~MeleeEnemyScript();
};

