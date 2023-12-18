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

    // ��ȯ ���
    XMMATRIX  m_matView;
    XMMATRIX  m_matProj;

public:
    virtual void FinalTick() override;

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

