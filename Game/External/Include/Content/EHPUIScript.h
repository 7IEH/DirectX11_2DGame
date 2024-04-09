#pragma once
#include <EHScript.h>
#include "EHRecordManager.h"

class PUIScript :
    public Script
{
private:
    /******************
    |   HP UI
    ******************/
    GameObject* m_pHPUI;
    GameObject* m_pHPHeartUI;
    Vec4        m_vHPUIScale;
    Vec4        m_vHPUIPos;

    /******************
    |   Weapon UI
    ******************/
    GameObject* m_pWeaponCircle1;
    GameObject* m_pWeaponCircle2;
    GameObject* m_pSetIcon1;
    GameObject* m_pSetIcon2;

    PlayerPref* m_pPlayerPref;

public:
    virtual void Awake() override;
    virtual void Update() override;
    
public:
    CLONE(PUIScript);
    PUIScript();
    virtual ~PUIScript();
};
