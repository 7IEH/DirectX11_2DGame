#pragma once
#include <EHScript.h>

enum class ATK1State
{
    Idle,
    Attack,
    none,
};

class GolemKingATK1Script :
    public Script
{
private:
    ATK1State m_eState;

    Vec2     m_vTargetPos;
    float    m_fAccTime;
    bool     m_bDown;
    bool     m_bUp;


public:
    virtual void Start() override;
    virtual void Update() override;

private:
    void Idle();
    void Attack();

    void SpawnCrack();

public:
    void SetState(ATK1State _eState) { m_eState = _eState; }
    void SetTargetPos(Vec2 _vTargetPos) { m_vTargetPos = _vTargetPos; }

public:
    CLONE(GolemKingATK1Script)
    GolemKingATK1Script();
    virtual ~GolemKingATK1Script();
};

