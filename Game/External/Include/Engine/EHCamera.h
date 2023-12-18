#pragma once

#include "EHComponent.h"

class Camera
	:public Component
{
private:
    PROJECTION_TYPE    m_Projection;

    // 원근투영(Perspective)
    float   m_FOV;          // 시야 각

    // 직교투영(Orthographic)
    float   m_Width;        // 직교투영 가로 길이
    float   m_Scale;        // 직교투영 배율

    // Both
    float   m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float   m_Far;          // 투영 최대 거리

    // 변환 행렬
    XMMATRIX  m_matView;
    XMMATRIX  m_matProj;

    GameObject* m_Target;

public:
    virtual void Tick()override;

public:
    void SetTarget(GameObject* _target) { m_Target = _target; }

public:
    XMMATRIX GetMatView() { return m_matView; }
    XMMATRIX GetMatProj() { return m_matProj; }

private:
    void ProjectiveView();
    void OrthographicView();

public:
    Camera();
    virtual ~Camera();
};

