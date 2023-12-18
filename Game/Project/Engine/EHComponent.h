#pragma once
#include "EHEntity.h"
class GameObject;
class Component :
    public Entity
{
private:
    COMPONENT_TYPE  m_Type;
    GameObject*     m_Owner;
  
public:
    COMPONENT_TYPE  GetType() { return m_Type; }
    GameObject*     GetOwner() { return m_Owner; }

    void            SetOwner(GameObject* _owner) { m_Owner = _owner; }
    
public:
    virtual void Tick() {};
    virtual void FinalTick() = 0;
    virtual void UpdateData() {};

public:
    Component(COMPONENT_TYPE _type);
    virtual ~Component();
};

