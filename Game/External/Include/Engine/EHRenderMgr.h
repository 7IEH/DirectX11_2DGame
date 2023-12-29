#pragma once

class RenderMgr
	:public Singleton<RenderMgr>
{
	Single(RenderMgr)
private:
	GameObject* m_Cam[(UINT)CAMERA_TYPE::END];

public:
	void RegisterCamera(CAMERA_TYPE _type, GameObject* _cam) { m_Cam[(UINT)_type] = _cam; }

public:
	void Update();
	
private:
	void Render();
	void DebugRender();
};

