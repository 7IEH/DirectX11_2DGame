#pragma once
#include "EHComponent.h"

enum class BUTTON_STATE
{
    HIGHLIGHT,
    PRESSED,
    SELECTED,
    DISABLED,
    END
};

class Button :
    public Component
{
private:
    BUTTON_STATE m_State;

public:
    virtual void Update()override;
    virtual void LateUpdate()override;

public:
    Button();
    virtual ~Button();
};

