#pragma once

#include "EHEntity.h"
#include "EHLayer.h"
class GameObject;
class Layer;
class Level
	:public Entity
{
private:
	Layer* m_Layers[(UINT)LAYER_TYPE::END];

public:
	void AddObject(GameObject* _obj, LAYER_TYPE _type, bool _bChildMove = true)
	{
		m_Layers[(UINT)_type]->AddObject(_obj,_bChildMove);
	}

	Layer* GetLayer(LAYER_TYPE _type) { return m_Layers[(UINT)_type]; }

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render();

private:
	void Clear();

public:
	Level();
	virtual ~Level();

	friend class LevelMgr;
};

