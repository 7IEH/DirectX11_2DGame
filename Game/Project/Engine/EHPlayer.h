#pragma once
#include "EHGameObject.h"
class Player :
    public GameObject
{
public:
    virtual void Tick()override;
    virtual void Render()override;
};

