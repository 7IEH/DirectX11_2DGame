#pragma once

#include "EHMeleeEnemyScript.h"

class BabySlimeScript :
    public MeleeEnemyScript
{
public:
    virtual void Update() override;

public:
    virtual void Idle()override;
    virtual void Chase()override;

    virtual void OnTriggerEnter(Collider* _other)override;

public:
    CLONE(BabySlimeScript);
    BabySlimeScript();
    virtual ~BabySlimeScript();

};

