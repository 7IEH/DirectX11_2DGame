#pragma once
#include <EHScript.h>

enum class SpawnRockShape
{
    Floor,
    Cake,
    None,
};

enum class SpawnRockState
{
    Idle,
    Prepare,
    Spawn,
    End,
};

class GolemKingATK2Script :
    public Script
{
private:
    SpawnRockShape m_eShape;
    SpawnRockState m_eState;

    Vec2           m_vTargetPos;

public:
    virtual void Start();
    virtual void Update();
    virtual void LateUpdate();

public:
    void StartLogic()
    {
        m_eState = SpawnRockState::Prepare;
    }

    void SetTargetPos(Vec2 _vTargetPos) { m_vTargetPos = _vTargetPos; }

private:
    void Prepare();
    void Spawn();
    
    void SpawnIndividualFloor(float _fRadius, int _idx,OUT vector<GameObject*>& _vObj);
    void SpawnIndividualCake(float _fRadian, OUT vector<GameObject*>& _vObj);
    void SpawnShadow(Vec4 _vPosition);

public:
    CLONE(GolemKingATK2Script);
    GolemKingATK2Script();
    virtual ~GolemKingATK2Script();
};

