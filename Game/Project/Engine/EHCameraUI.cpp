#include "pch.h"
#include "EHCameraUI.h"

#include "EHGameObject.h"
#include "EHCamera.h"

CameraUI::CameraUI()
	:ComponentUI("Camera","##Camera",COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Camera* _cam = GetTargetObject()->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
}