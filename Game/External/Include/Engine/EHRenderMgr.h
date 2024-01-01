#pragma once

class GameObject;

class RenderMgr
	:public Singleton<RenderMgr>
{
	Single(RenderMgr)
private:
	GameObject* m_Cam[(UINT)CAMERA_TYPE::END];

	list<tDebugShapeInfo>   m_DbgShapeInfo;

	GameObject* m_pDebugObj;

public:
	void RegisterCamera(CAMERA_TYPE _type, GameObject* _cam) { m_Cam[(UINT)_type] = _cam; }
	void AddDebugShapeInfo(const tDebugShapeInfo& _info)
	{
		m_DbgShapeInfo.push_back(_info);
	}

public:
	void Awake();
	void Update();
	
private:
	void Render();
	void DebugRender();
};

