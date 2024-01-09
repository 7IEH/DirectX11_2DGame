#include "pch.h"
#include "EHRenderMgr.h"

#include "EHGameObject.h"
#include "EHTransform.h"
#include "EHMeshRenderer.h"

#include "EHStructuredBuffer.h"

void RenderMgr::Awake()
{
	m_pDebugObj = new GameObject;
	m_pDebugObj->AddComponent<Transform>();
	m_pDebugObj->AddComponent<MeshRenderer>();

	m_Light2DBuffer = new StructuredBuffer;
	m_Light2DBuffer->Create(sizeof(LightInfo), 10, TRUE);
}