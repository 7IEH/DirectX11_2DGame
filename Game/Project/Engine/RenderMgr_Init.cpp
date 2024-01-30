#include "pch.h"
#include "EHRenderMgr.h"

#include "EHAssetMgr.h"

#include "EHGameObject.h"
#include "EHTransform.h"
#include "EHMeshRenderer.h"

#include "EHStructuredBuffer.h"

#include "EHDevice.h"

void RenderMgr::Awake()
{
	m_pDebugObj = new GameObject;
	m_pDebugObj->AddComponent<Transform>();
	m_pDebugObj->AddComponent<MeshRenderer>();

	m_PickingObj = new GameObject;
	m_PickingObj->AddComponent<Transform>();
	m_PickingObj->AddComponent<MeshRenderer>();

	m_Light2DBuffer = new StructuredBuffer;
	m_Light2DBuffer->Create(sizeof(LightInfo), 10, TRUE);

	Vec2 _renderResoultion = Device::GetInst()->GetResolution();
	m_PostProcessTex = AssetMgr::GetInst()->CreateResoruceTexture(L"PostProcessTexture"
																, (UINT)1600.f
																, (UINT)900.f
																, DXGI_FORMAT_R8G8B8A8_UNORM
																, D3D11_BIND_SHADER_RESOURCE);

	m_vecNoiseTex.push_back(AssetMgr::GetInst()->Load<Sprite>(L"\\resource\\noise_01.png", L"\\resource\\noise_01.png"));
	//m_vecNoiseTex.push_back(AssetMgr::GetInst()->Load<Sprite>(L"\\texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png"));
	//m_vecNoiseTex.push_back(AssetMgr::GetInst()->Load<Sprite>(L"\\texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));

	m_vecNoiseTex[0]->UpdateData(14);
	m_vecNoiseTex[0]->UpdateData_CS_SRV(14);
}