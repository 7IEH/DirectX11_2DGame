#pragma once
#include "EHComponent.h"
class RigidBody :
    public Component
{
private:
    float   m_fMass;
    float   m_fFriction;
    float   m_fCoefficient;

    Vec2    m_vVelocity;
    Vec2    m_vAccelation;
    Vec2    m_vForce;
    Vec2    m_vGravity;
    Vec2    m_vMaxGravity;

public:
    void SetVeclocity(Vec2 _vVelocity) { m_vVelocity = _vVelocity; }
    Vec2 GetVelocity() { return m_vVelocity; }
    
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }

    void Clear() { m_vForce =Vec2(0.f, 0.f); }

public:
    virtual void Update()override;
    virtual void LateUpdate() override;
    virtual void UpdateData() override;

    virtual void Save(std::wofstream* _file)override;
    virtual void Load(std::wifstream* _file)override;

    CLONE(RigidBody)
public:
    RigidBody();
    virtual ~RigidBody();
};

