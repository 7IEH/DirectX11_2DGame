#pragma once
#include <EHScript.h>
class SpawnRockScript :
    public Script
{
private:
    float m_fTarget;
    float m_fAcctime;

    bool  m_bStart;
    bool  m_bFadeOut;
    bool  m_bDead;

public:
    virtual void Update();

public:
    void SetTarget(float _fTarget) { m_fTarget = _fTarget; }

public:
    CLONE(SpawnRockScript)
    SpawnRockScript();
    virtual ~SpawnRockScript();
};

