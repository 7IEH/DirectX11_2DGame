#pragma once

#include "EHEntity.h"

#include "EHGameObject.h"

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

	GameObject* FindObject(const wstring& _strName)
	{
		for (size_t i = 0;i < m_Parent.size();i++)
		{
			if (m_Parent[i]->GetName() == _strName)
			{
				return m_Parent[i];
			}
		}

		return nullptr;
	}

public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	
	CLONE(Layer);

public:
	vector<GameObject*>& GetLayerObject() { return m_GameObjects; }
	vector<GameObject*>& GetLayerParent() { return m_Parent; }

private:
	void Save(std::wofstream* _file);

public:
	Layer();
	Layer(const Layer& _origin);
	virtual ~Layer();

	friend class Level;
	friend class LevelMgr;
};

