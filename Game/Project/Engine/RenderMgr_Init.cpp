#include "pch.h"
#include "EHRenderMgr.h"

#include "EHGameObject.h"
#include "EHTransform.h"
#include "EHMeshRenderer.h"

void RenderMgr::Awake()
{
	m_pDebugObj = new GameObject;
	m_pDebugObj->AddComponent<Transform>();
	m_pDebugObj->AddComponent<MeshRenderer>();
}