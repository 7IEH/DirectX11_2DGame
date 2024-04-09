#pragma once
#include "EHMeleeEnemyScript.h"
class GolemButlerScript :
    public MeleeEnemyScript
{
private:

public:
    virtual void Update() override;

public:
    virtual void Idle()override;
    virtual void Chase()override;
    virtual void Attack()override;

    void OnTriggerEnter(Collider* _other)override;

    void OnTriggerExit(Collider* _other)override;
   
public:
    CLONE(GolemButlerScript);
    GolemButlerScript();
    virtual ~GolemButlerScript();

};