#pragma once
#include <EHScript.h>
class SlotScript :
    public Script
{
private:
    int         m_iCurSlot;
    bool        m_bTrans;

    GameObject* m_pUnderLine1;
    GameObject* m_pUnderLine2;

    GameObject* m_pSelectIcon1;
    GameObject* m_pSelectIcon2;

    Vec4        m_vUnderLinePos1;
    Vec4        m_vUnderLinePos2;

    Vec4        m_vUnderLineScale1;
    Vec4        m_vUnderLineScale2;

    Vec4        m_vSelectIconPos1;
    Vec4        m_vSelectIconPos2;

    vector<std::pair<GameObject*, GameObject*>>m_Slots;
    vector<GameObject*> m_SlotNames;

public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
public:
    CLONE(SlotScript)

    SlotScript();
    virtual ~SlotScript();
};

