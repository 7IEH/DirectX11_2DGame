#pragma once
#include <EHScript.h>

class GolemKingATK4Script :
    public Script
{
public:
    virtual void OnTriggerEnter(Collider* _other)override;

public:
    CLONE(GolemKingATK4Script)
    GolemKingATK4Script();
    virtual ~GolemKingATK4Script();
};

