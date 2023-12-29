#pragma once

#include "EHEntity.h"

class GameObject;
class Layer
	:public Entity
{
private:
	vector<GameObject*>			m_Parent;
	vector<GameObject*>			m_GameObjects;

	LAYER_TYPE					m_Type;

public:
	void AddObject(GameObject* _obj, bool _bMove);
	void DetachGameObject(GameObject* _obj);
	void RegisterGameObject(GameObject* _obj) { m_GameObjects.push_back(_obj); }

public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	
public:
	vector<GameObject*>& GetLayerObject() { return m_GameObjects; }
	vector<GameObject*>& GetLayerParent() { return m_Parent; }
public:
	Layer();
	virtual ~Layer();

	friend class Level;
};

