#pragma once
#include <EHScript.h>

enum class SceneState
{
    Start,
    Intro,
    Idle,
    End,
};

class GolemBossSceneScript :
    public Script
{
private:
    SceneState m_eSceneState;

    GameObject* m_pGolemBoss;
    GameObject* m_pPlayer;
    GameObject* m_pBossDoor;

    GameObject* m_pCamera;

    /*************
    |   Intro
    *************/
    bool m_bIntro;
    bool m_bIntro2;
    bool m_bIntro3;
    GameObject* m_pPostProcessing;
    
    /**********
    |   Boss UI
    ***********/
    GameObject* m_pNameText;
    GameObject* m_pScroll;
    GameObject* m_pHealthBar;
    GameObject* m_pHealthBarDeco;

    /***********
    | EndScene
    ***********/
    bool m_bEnd;


    float       m_fTime;

public:
    virtual void Start()override;
    virtual void Update()override;
    virtual void LateUpdate() override;

private:
    void Enter();
    void Intro();
    void Idle();
    void End();

public:
    CLONE(GolemBossSceneScript)
    GolemBossSceneScript();
    virtual ~GolemBossSceneScript();
};

