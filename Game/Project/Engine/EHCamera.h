#pragma once

#include "EHComponent.h"

class Camera
	:public Component
{
private:
    PROJECTION_TYPE    m_Projection;

    // ��������(Perspective)
    float   m_FOV;          // �þ� ��

    // ��������(Orthographic)
    float   m_Width;        // �������� ���� ����
    float   m_Scale;        // �������� ����

    // Both
    float   m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
    float   m_Far;          // ���� �ִ� �Ÿ�

public:
    virtual void LateUpdate() override;

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

