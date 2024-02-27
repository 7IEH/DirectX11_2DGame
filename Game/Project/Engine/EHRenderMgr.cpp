#include "pch.h"
#include "EHRenderMgr.h"

#include "EHGameObject.h"
#include "EHDevice.h"

#include "EHCamera.h"

#include "EHMeshRenderer.h"
#include "EHTransform.h"

#include "EHAssetMgr.h"
#include "EHTimeMgr.h"
#include "EHImGUIMgr.h"
#include "EHLevelMgr.h"

#include "EHLevel.h"

#include "EHLIght2D.h"

#include "EHStructuredBuffer.h"
#include "EHConstantBuffer.h"

#include "EHDevice.h"

#include "EHKeyMgr.h"

#include "EHFontMgr.h"

extern transform e_MatrixData;

RenderMgr::RenderMgr()
	:m_Cam{}
	, m_NotRender(TRUE)
	, m_Light2DBuffer(nullptr)
	, m_pDebugObj(nullptr)
	, m_PickingObj(nullptr)
	, m_multiPort(0)
{
}

RenderMgr::~RenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;

	if (nullptr != m_PickingObj)
		delete m_PickingObj;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void RenderMgr::Update()
{
	// RenderTarget, DepthStencil ÃÊ±âÈ­
	Device::GetInst()->OMSetRT();

	float ClearColor[4] = { 0.3f,0.3f,0.3f,0.3f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

#ifdef _DEBUG
	for (int i = 0;i < 2;i++)
	{
		if (i == 0)
		{
			Device::GetInst()->CreateViewPort(Vec2(0.f, 0.f), Vec2(800.f, 900.f));
			UpdateData();
			Multi_Render();
		}
		else
		{
			Device::GetInst()->CreateViewPort(Vec2(800.f, 0.f), Vec2(800.f, 900.f));
			UpdateData();
			Render();
		}
	}

	PickingRender();

	DebugRender();
#else

	UpdateData();
	Render();

#endif

	Clear();

	CopyResourceView();

#ifdef _DEBUG
	ImGUIMgr::GetInst()->Render();
#endif

	Device::GetInst()->Present();
}

void RenderMgr::Render()
{
	for (int i = 0; i < (UINT)CAMERA_TYPE::END;i++)
	{
		if (i == (UINT)CAMERA_TYPE::WORLD_CAMERA)
			continue;
		if (nullptr == m_Cam[i])
			continue;

		Camera* _cam = m_Cam[i]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
		_cam->SortObject();
		_cam->Render();
	}
}

void RenderMgr::Multi_Render()
{
	if (nullptr == m_Cam[(UINT)CAMERA_TYPE::WORLD_CAMERA])
		return;

	Camera* _cam = m_Cam[(UINT)CAMERA_TYPE::WORLD_CAMERA]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
	_cam->SortObject();
	_cam->Render();
}

void RenderMgr::DebugRender()
{
	Camera* _main_cam = m_Cam[0]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
	e_MatrixData.View = _main_cam->GetViewMat();
	e_MatrixData.Projection = _main_cam->GetProjMat();

	MeshRenderer* _debugMeshRenderer = m_pDebugObj->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER);
	Transform* _debugtr = m_pDebugObj->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end();)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			_debugMeshRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			_debugMeshRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultCircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CUBE:
			_debugMeshRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultCubeMesh_Debug"));
			break;
		case DEBUG_SHAPE::SPHERE:
			_debugMeshRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultSphereMesh_Debug"));
			break;
		default:
			break;
		}

		_debugMeshRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"DebugMaterial"));
		_debugMeshRenderer->GetMaterial()->SetMaterialParam(AMBIENT, Vec4((*iter).vColor));

		_debugtr->SetWorldMat((*iter).matWorld);
		_debugtr->UpdateData();

		m_pDebugObj->Render();

		(*iter).fLifeTime += DT;
		if ((*iter).fLifeTime >= (*iter).fDuration)
		{
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void RenderMgr::PickingRender()
{
	Camera* _main_cam = m_Cam[0]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
	e_MatrixData.View = _main_cam->GetViewMat();
	e_MatrixData.Projection = _main_cam->GetProjMat();

	MeshRenderer* _pickingMeshRenderer = m_PickingObj->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER);
	Transform* _pickingTr = m_PickingObj->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	_pickingMeshRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh_Debug"));

	_pickingMeshRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"DebugMaterial"));
	_pickingMeshRenderer->GetMaterial()->SetMaterialParam(AMBIENT, Vec4(0.f, 1.f, 0.f, 1.f));

	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	for (size_t _idx = 0;_idx < (UINT)LAYER_TYPE::END;_idx++)
	{
		Layer* _layer = _curLevel->GetLayer(LAYER_TYPE(_idx));
		vector<GameObject*>& _objs = _layer->GetLayerObject();

		for (size_t _obj = 0;_obj < _objs.size();_obj++)
		{
			if (_objs[_obj]->GetPicking() == TRUE)
			{
				Transform* _ownertr = _objs[_obj]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
				if (_ownertr == nullptr)
					return;

				_pickingTr->SetWorldMat(_ownertr->GetMatWorld());
				m_PickingObj->Render();
			}
		}
	}
}

void RenderMgr::UpdateData()
{
	static vector<LightInfo> _vecLight2D = {};
	NomralVector temp = {};
	for (int i = 0;i < m_Light.size();i++)
	{
		LIght2D* _light2D = m_Light[i]->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);
		_vecLight2D.push_back(_light2D->GetLightInfo());

		Transform* _tr = m_Light[i]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	}

	m_Light2DBuffer->SetData(_vecLight2D.data(), (UINT)_vecLight2D.size());
	m_Light2DBuffer->UpdateData(11);

	e_Global._Light2DSize = (int)m_Light.size();

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::GLOBAL)->SetData(&e_Global, sizeof(e_Global), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::GLOBAL)->UpdateData();

	_vecLight2D.clear();
}

void RenderMgr::Clear()
{
	m_Light.clear();
}

void RenderMgr::CopyResourceView()
{
	CONTEXT->CopyResource(m_PostProcessTex.Get()->GetTexture2D(), Device::GetInst()->GetRTT());
}