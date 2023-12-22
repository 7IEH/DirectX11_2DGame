#pragma once
#include "EHEntity.h"
#include "EHComponent.h"
#include "EHTransform.h"
#include "EHMeshRenderer.h"
#include "EHRenderer.h"
#include "EHCamera.h"

#include "EHScript.h"
#include "EHPlayerScript.h"
#include "EHCameraScript.h"
#include "EHCameraTargetScript.h"

class Component;
class Script;

class GameObject
	: public Entity
{
private:
	Component* m_Component[(UINT)COMPONENT_TYPE::END];
	Component* m_Renderer;

	vector<Script*> m_vScripts;

	vector<GameObject*> m_Childs[(UINT)LAYER_TYPE::END];
	GameObject*			m_Parent;

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

	template<typename T>
	T* GetComponent(COMPONENT_TYPE _type)
	{
		T* _comp = dynamic_cast<T*>(m_Component[(UINT)_type]);
		if (_comp != nullptr)
			return _comp;
	}

	vector<Script*>& GetScript()
	{
		return m_vScripts;
	}

	GameObject* GetParent() { return m_Parent; }

	void AddChild(GameObject* _child, LAYER_TYPE _type)
	{
		if (_child->m_Parent != nullptr)
		{
			_child->m_Parent = nullptr;
			vector<GameObject*>::iterator iter = m_Parent->m_Childs[(UINT)_type].begin();
			for (;iter != m_Parent->m_Childs[(UINT)_type].end();)
			{
				if ((*iter) == _child)
				{
					m_Parent->m_Childs[(UINT)_type].erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}
		m_Childs[(UINT)_type].push_back(_child);
		_child->m_Parent = this;
	}

public:
	virtual void Init();
	virtual void Tick();
	virtual void FinalTick();
	virtual void Render();

public:
	GameObject();
	virtual ~GameObject();
};

