#pragma once
#include "EHGameObject.h"
class Player :
    public GameObject
{
public:
    virtual void Tick()override;
    virtual void FinalTick() override;
    virtual void Render()override;
};

