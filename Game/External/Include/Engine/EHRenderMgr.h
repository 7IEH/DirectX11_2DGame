#pragma once

class GameObject;
class StructuredBuffer;

#include "EHSprite.h"

class RenderMgr
	:public Singleton<RenderMgr>
{
	Single(RenderMgr)
private:
	// CAM
	GameObject*					m_Cam[(UINT)CAMERA_TYPE::END];
	
	// PostProcess Texture
	Ptr<Sprite>					m_PostProcessTex;

	// Light
	StructuredBuffer*			m_Light2DBuffer;
	vector<GameObject*>			m_Light;

	list<tDebugShapeInfo>		m_DbgShapeInfo;

	GameObject*					m_pDebugObj;

	bool						m_NotRender;

public:
	void SetRender(bool _NotRender) { m_NotRender = _NotRender; }
	bool GetRender() { return m_NotRender; }

	void RegisterCamera(CAMERA_TYPE _type, GameObject* _cam) { m_Cam[(UINT)_type] = _cam; }
	void AddDebugShapeInfo(const tDebugShapeInfo& _info)
	{
		m_DbgShapeInfo.push_back(_info);
	}

	void RegisterLight2D(GameObject* _light) { m_Light.push_back(_light); }

	Ptr<Sprite>		GetPostProcessTexture2D() { return m_PostProcessTex; }

public:
	void Awake();
	void Update();
	
private:
	void Render();
	void DebugRender();

	void UpdateData();
	void Clear();

	void CopyResourceView();
};

