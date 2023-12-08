#pragma once
#include "EHEntity.h"
class Component :
    public Entity
{
private:
    COMPONENT_TYPE m_Type;

public:
    Component(COMPONENT_TYPE _type);
    virtual ~Component();

public:
    COMPONENT_TYPE GetType() { return m_Type; }

public:
    virtual void Create() = 0;

public:
    virtual void Tick() = 0;
    virtual void Render() = 0;
};

