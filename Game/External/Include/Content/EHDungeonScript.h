#pragma once
#include "EHScript.h"

class DungeonScript :
    public Script
{
private:
    DungeonType m_iCurType;
    GameObject* m_pMainCam;

public:
    void SetDungeonType(DungeonType _iCurType) { m_iCurType = _iCurType; }

public:
    virtual void Start()override;
    virtual void Update()override;
    virtual void LateUpdate()override;

public:
    CLONE(DungeonScript)

    DungeonScript();
    virtual ~DungeonScript();
};

