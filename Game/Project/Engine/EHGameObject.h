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

class Component;
class Script;

class GameObject
	: public Entity
{
private:
	Component*		m_Component[(UINT)COMPONENT_TYPE::END];
	Component*		m_Renderer;

	vector<Script*> m_vScripts;

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

public:
	virtual void Init();
	virtual void Tick();
	virtual void FinalTick();
	virtual void Render();

public:
	GameObject();
	virtual ~GameObject();
};

