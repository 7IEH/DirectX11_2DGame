#include "pch.h"
#include "EHRenderMgr.h"

#include "EHGameObject.h"
#include "EHDevice.h"

RenderMgr::RenderMgr()
	:m_Cam{}
{

}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::Update()
{
	// RenderTarget, DepthStencil ÃÊ±âÈ­
	float ClearColor[4] = { 1.f,1.f,1.f,1.f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

	Render();
	DebugRender();

	Device::GetInst()->Present();
}

void RenderMgr::Render()
{
	for (int i = 0; i < (UINT)CAMERA_TYPE::END;i++)
	{
		if (m_Cam[i] == nullptr)
			continue;

		Camera* _cam  = m_Cam[i]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
		_cam->Render();
	}
}

void RenderMgr::DebugRender()
{

}
