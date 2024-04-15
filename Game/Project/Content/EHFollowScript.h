#pragma once
#include <EHScript.h>
class FollowScript :
    public Script
{
private:
    GameObject* m_pFollowingObj;
    Vec4        m_vOffSetPos;

public:
    virtual void Update()override;

public:
    void SetFollowObject(GameObject* _pFollowObj) { m_pFollowingObj = _pFollowObj; }
    void SetOffsetPos(Vec4 _vOffsetPos) { m_vOffSetPos = _vOffsetPos; }

public:
    CLONE(FollowScript)
    FollowScript();
    virtual ~FollowScript();
};

