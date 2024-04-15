#pragma once
#include "EHMeleeEnemyScript.h"
class GolemSoilderScript :
    public MeleeEnemyScript
{
private:
    EnemyDir m_eAttackDir;
    float   m_fAccTime;
    GameObject* m_pCollider;

public:
    virtual void Update() override;

public:
    virtual void Start()override;
    virtual void Idle()override;
    virtual void Chase()override;
    virtual void Attack() override;

    virtual void OnTriggerEnter(Collider* _other)override;

public:
    CLONE(GolemSoilderScript)
    GolemSoilderScript();
    virtual ~GolemSoilderScript();
};

