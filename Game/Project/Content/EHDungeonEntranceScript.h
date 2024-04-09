#pragma once
#include <EHScript.h>
class DungeonEntranceScript :
    public Script
{
private:
    GameObject* m_pTownTrigger;
    GameObject* m_pDungeonTrigger;

public:
    virtual void Start() override;
    virtual void Update() override;
public:
    CLONE(DungeonEntranceScript)
    DungeonEntranceScript();
    virtual ~DungeonEntranceScript();
};

