#pragma once
#include "EHDungeonScript.h"
class GolemDungeonScript :
    public DungeonScript
{
private:
    GameObject* m_pPlayer;

    // Intro Object
    bool        m_bIntro1;
    bool        m_bIntro2;
    GameObject* m_pScroll;
    GameObject* m_pIntroBG;
    GameObject* m_pIntroText;
    GameObject* m_pIntroDoor;
    GameObject* m_pPostProcessing;

public:
    virtual void Start()override;
    virtual void Update()override;
    virtual void LateUpdate()override;

public:
    CLONE(GolemDungeonScript)

    GolemDungeonScript();
    virtual ~GolemDungeonScript();
};

