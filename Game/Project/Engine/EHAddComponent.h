#pragma once
#include "EHComponentUI.h"
class AddComponent :
    public UI
{
private:
    bool    m_Open;
    GameObject* m_TargetObject;

public:
    virtual void Render_Update()override;

public:
    void SetTargetObject(GameObject* _targetObject) { m_TargetObject = _targetObject; }

public:
    AddComponent();
    virtual ~AddComponent();
};

