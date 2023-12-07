#pragma once
#include "EHEntity.h"

class Component;

class GameObject
	: public Entity
{
private:
	vector<Component*> m_Component;

public:
	void Init();
	void Tick();
	void Render();
};

