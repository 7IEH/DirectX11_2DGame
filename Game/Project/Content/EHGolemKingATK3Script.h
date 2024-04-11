#pragma once
#include <EHScript.h>

enum class StickyState
{   
    Idle,
    Prepare,
    Cycle1,
    Cycle2,
    Launch,
    End,
};

enum class SwingDir
{
    Left,
    Right,
};

class GolemKingATK3Script :
    public Script
{
private:
    StickyState m_eState;
    GameObject* m_pPlayer;

    float       m_fAimTime;
    GameObject* m_pFist;
    GameObject* m_pFistModule;
    GameObject* m_pFistModule2;
    GameObject* m_pCam;

    float       m_fLaunchRadian;
    float       m_fLaunchRadius;

    bool        m_bCycle;

    SwingDir        m_eSwing;
    float           m_fSpeed;

public:
    virtual void Start() override;
    virtual void Update() override;

public:
    void StartLogic()
    {
        m_eState = StickyState::Prepare;
    }

private:
    void Prepare();
    void Cycle1();
    void Cycle2();
    void Launch();
    void End();

public:
    CLONE(GolemKingATK3Script)
    GolemKingATK3Script();
    virtual ~GolemKingATK3Script();
};

