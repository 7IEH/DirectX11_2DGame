#pragma once

#include "EHComponent.h"

class Camera
	:public Component
{
private:
    PROJECTION_TYPE    m_Projection;

    // 원근투영(Perspective)
    float               m_FOV;          // 시야 각

    // 직교투영(Orthographic)
    float               m_Width;        // 직교투영 가로 길이
    float               m_Scale;        // 직교투영 배율

    // Both
    float               m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float               m_Far;          // 투영 최대 거리

    // 1. View Matrix
    // 2. Projection Matrix
    Matrix              m_ViewMat;
    Matrix              m_ProjMat;

    UINT                m_LayerVisible;
    CAMERA_TYPE         m_Type;

public:
    // Layer
    void LayerVisibleSet(LAYER_TYPE _type, bool _visible);
    void AllVisibleSet(bool _visible);
    void SetCameraType(CAMERA_TYPE _type);

public:
    virtual void LateUpdate() override;
    void Render();

public:
    PROJECTION_TYPE GetProjectionType() { return m_Projection; }
    void            SetPorjectionType(PROJECTION_TYPE _projection) { m_Projection = _projection; }

    void            InitializeDir();

private:
    void ProjectiveView();
    void OrthographicView();

public:
    Camera();
    virtual ~Camera();
};

