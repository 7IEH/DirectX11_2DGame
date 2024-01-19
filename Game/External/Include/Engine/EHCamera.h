#pragma once

#include "EHComponent.h"

class Camera
	:public Component
{
private:
	PROJECTION_TYPE                 m_Projection;

	// ��������(Perspective)
	float                           m_FOV;          // �þ� ��

	// ��������(Orthographic)
	float                           m_Width;        // �������� ���� ����
	float                           m_Height;        // �������� ���� ����
	float                           m_Scale;        // �������� ����

	// Both
	float                           m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
	float                           m_Far;          // ���� �ִ� �Ÿ�

	// 1. View Matrix
	// 2. Projection Matrix
	Matrix                          m_ViewMat;
	Matrix                          m_ProjMat;

	UINT                            m_LayerVisible;
	CAMERA_TYPE                     m_Type;

	vector<GameObject*>             m_DomainSortingObjects[(UINT)SHADER_DOMAIN::END];

public:
	// Layer
	void LayerVisibleSet(LAYER_TYPE _type, bool _visible);
	void AllVisibleSet(bool _visible);
	void SetCameraType(CAMERA_TYPE _type);

	void SetFOV(float _FOV) { m_FOV = _FOV; }
	float GetFOV() { return m_FOV; }

	void SetFar(float _far) { m_Far = _far; }
	float GetFar() { return m_Far; }

	void SetScale(float _scale) { m_Scale = _scale; }
	float GetScale() { return m_Scale; }

public:
	virtual void LateUpdate() override;

	void SortObject();
	void Render();

private:
	void Render(vector<GameObject*>& _vecObj);
	void PostRender(vector<GameObject*>& _vecObj);

public:
	PROJECTION_TYPE GetProjectionType() { return m_Projection; }
	void            SetPorjectionType(PROJECTION_TYPE _projection) { m_Projection = _projection; }

	void            InitializeDir();

	Matrix          GetViewMat() { return m_ViewMat; }
	Matrix          GetProjMat() { return m_ProjMat; }

private:
	void ProjectiveView();
	void OrthographicView();

public:
	Camera();
	virtual ~Camera();
};

