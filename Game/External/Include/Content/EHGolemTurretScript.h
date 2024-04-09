#pragma once
#include "EHRangedEnemyScript.h"
class GolemTurretScript :
    public RangedEnemyScript
{
public:
    virtual void Update() override;

public:
    virtual void Idle()override;
    virtual void Chase()override;
    virtual void Attack()override;
    
    virtual void OnTriggerEnter(Collider* _other)override;
public:
    CLONE(GolemTurretScript);
    GolemTurretScript();
    virtual ~GolemTurretScript();
};

