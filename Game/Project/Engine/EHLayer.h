#pragma once

#include "EHEntity.h"

class GameObject;
class Layer
	:public Entity
{
private:
	vector<GameObject*> m_GameObjects;

public:
	void AddObject(GameObject* _obj)
	{
		m_GameObjects.push_back(_obj);
	}

public:
	void Init();
	void Tick();
	void FinalTick();
	void Render();

public:
	Layer();
	virtual ~Layer();
};

