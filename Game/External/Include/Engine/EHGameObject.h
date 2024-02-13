#pragma once
#include "EHEntity.h"
#include "EHBehaviour.h"

class Component;
class Script;

class GameObject
	: public Entity
{
private:
	LAYER_TYPE					m_LayerType;

	Component*					m_Component[(UINT)COMPONENT_TYPE::END];
	Component*					m_Renderer;

	vector<Script*>				m_vScripts;
	vector<GameObject*>			m_Childs;

	GameObject*					m_Parent;

	bool						m_Dead;
	bool						m_Picking;

	static int					m_ObjectID;

	int							m_Idx;
public:
	template<typename T>
	T* AddComponent()
	{
		T* _comp = new T();
		if (_comp->GetType() == COMPONENT_TYPE::SCRIPT)
		{
			m_vScripts.push_back((Script*)_comp);
			_comp->SetOwner(this);
		}
		else
		{
			if (_comp->GetType() == COMPONENT_TYPE::RENDERER)
			{
				if (m_Renderer == nullptr)
				{
					m_Renderer = _comp;
				}
			}

			if (m_Component[(UINT)_comp->GetType()] == nullptr)
			{
				m_Component[(UINT)_comp->GetType()] = _comp;
				_comp->SetOwner(this);
			}
			else
			{
				delete _comp;
				_comp = nullptr;
				_comp = dynamic_cast<T*>(m_Component[(UINT)_comp->GetType()]);
			}
		}
		return _comp;
	}

public:
	LAYER_TYPE		GetLayerType() { return m_LayerType; }
	void			SetLayerType(LAYER_TYPE _type) { m_LayerType = _type; }

	template<typename T>
	T* GetComponent(COMPONENT_TYPE _type)
	{
		T* _comp = dynamic_cast<T*>(m_Component[(UINT)_type]);
		if (_comp != nullptr)
			return _comp;

		return _comp;
	}

	vector<Script*>& GetScript()
	{
		return m_vScripts;
	}

	GameObject* GetParent() { return m_Parent; }
	void AddChild(GameObject* _child);
	void DisconnectWithParent();
	void DisconnectWithLayer();

	void		SetDead(bool _dead) { m_Dead = _dead; }
	bool		GetDead() { return m_Dead; }

	void		SetPicking(bool _picking) { m_Picking = _picking; }
	bool		GetPicking() { return m_Picking; }

	int		GetIdx() { return m_Idx; }
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render();

private:
	void Save(string _path);

public:
	GameObject();
	virtual ~GameObject();

	friend class Layer;
	friend class TaskMgr;
};

