#pragma once
#include <EHScript.h>

class BossUIScript :
    public Script
{
private:
    GameObject* m_pHealthBar;
    GameObject* m_pHealthBarDeco;
    bool        m_bDead;

    Vec4        m_vHPUIScale;
    Vec4        m_vHPUIPos;

    bool        m_bAttack;
    float       m_fAttackTime;
    
public:
    virtual void Start();
    virtual void Update();

public:
    void SetDead(bool _bDead) { m_bDead = _bDead; }
    void SetAttack() { m_bAttack = TRUE; }

public:
    CLONE(BossUIScript)
    BossUIScript();
    virtual ~BossUIScript();
};

