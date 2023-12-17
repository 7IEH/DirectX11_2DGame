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
	void AddObject(GameObject* _obj, LAYER_TYPE _type)
	{
		m_Layers[(UINT)_type]->AddObject(_obj);
	}

public:
	virtual void Init();
	virtual void Tick();
	virtual void FinalTick();
	virtual void Render();

public:
	Level();
	virtual ~Level();
};

