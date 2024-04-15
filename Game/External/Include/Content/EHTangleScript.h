#pragma once
#include "EHMeleeEnemyScript.h"
class TangleScript :
    public MeleeEnemyScript
{
public:
    virtual void Update() override;

public:
    virtual void Start()override;
    virtual void Idle()override;
    virtual void Chase()override;

    virtual void OnTriggerEnter(Collider* _other)override;
public:
    CLONE(TangleScript);
    TangleScript();
    virtual ~TangleScript();
};

