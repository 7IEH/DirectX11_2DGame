#pragma once
#include "EHEntity.h"
#include "EHComponent.h"
#include "EHTransform.h"
#include "EHMeshRenderer.h"

class Component;

class GameObject
	: public Entity
{
private:
	Component* m_Component[(UINT)COMPONENT_TYPE::END];

public:
	template<typename T>
	T* AddComponent()
	{
		T* _comp = new T();
		if (m_Component[(UINT)_comp->GetType()] == nullptr)
		{
			_comp->Create();
			m_Component[(UINT)_comp->GetType()] = _comp;
		}
		else
		{
			_comp = dynamic_cast<T*>(m_Component[(UINT)_comp->GetType()]);
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

public:
	void Init();
	virtual void Tick();
	virtual void Render();
};

