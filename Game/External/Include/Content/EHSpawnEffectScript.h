#pragma once
#include <EHScript.h>
class SpawnEffectScript :
    public Script
{
private:
    float m_fDeleteTime;
    float m_fAcctime;
    bool  m_fFadeOut;

public:
    virtual void Update()override;

public:
    void SetDeleteTime(float _fDeleteTime) { m_fDeleteTime = _fDeleteTime; }

public:
    CLONE(SpawnEffectScript)
    SpawnEffectScript();
    virtual ~SpawnEffectScript();
};

