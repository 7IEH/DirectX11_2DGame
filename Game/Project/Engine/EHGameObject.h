#pragma once
#include "EHEntity.h"
#include "EHBehaviour.h"

class Component;
class Script;
class Level;
class GameObject
	: public Entity
{
private:
	bool						m_bEnabled;

	LAYER_TYPE					m_LayerType;

	Component* m_Component[(UINT)COMPONENT_TYPE::END];
	Component* m_Renderer;

	vector<Script*>				m_vScripts;
	vector<GameObject*>			m_Childs;

	GameObject* m_Parent;

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
				T* _temp = _comp;
				_comp = dynamic_cast<T*>(m_Component[(UINT)_comp->GetType()]);
				delete _comp;
				_comp = nullptr;
			}
		}
		return _comp;
	}

	void AddComponent(Component* _comp);

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

	vector<Script*>& GetScripts()
	{
		return m_vScripts;
	}

	template<typename T>
	T* GetScript(const wstring& _wScriptName)const
	{
		vector<Script*>::const_iterator iter = m_vScripts.begin();
		for (;iter != m_vScripts.end();iter++)
		{
			if (_wScriptName == (*iter)->GetName())
			{
				return dynamic_cast<T*>((*iter));
			}
		}

		return nullptr;
	}

	template<typename T>
	T* GetScript()const
	{
		vector<Script*>::const_iterator iter = m_vScripts.begin();
		for (;iter != m_vScripts.end();iter++)
		{
			T* _pScript = dynamic_cast<T*>((*iter));

			if (nullptr != _pScript)
			{
				return _pScript;
			}
		}

		return nullptr;
	}

	GameObject*					GetParent() { return m_Parent; }
	const vector<GameObject*>&	GetChild() { return m_Childs; }

	void						AddChild(GameObject* _child);
	int							DisconnectWithParent();
	int							DisconnectWithLayer();

	void						SetDead(bool _dead) { m_Dead = _dead; }
	bool						GetDead() { return m_Dead; }

	void						SetPicking(bool _picking) { m_Picking = _picking; }
	bool						GetPicking() { return m_Picking; }

	int							GetIdx() { return m_Idx; }

	void						Enabled(bool _flag) { m_bEnabled = _flag; }
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render();

	CLONE(GameObject);

	void Load(std::wifstream* _file, Level* _level, bool _bParent);

private:
	void Save(std::wofstream* _file);

public:
	GameObject();
	GameObject(const GameObject& _origin);
	virtual ~GameObject();

	friend class Layer;
	friend class TaskMgr;
};

