#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"

GameObject::GameObject()
	:m_Component{}
	,m_Renderer(nullptr)
{
}

GameObject::~GameObject()
{
	ReleaseArray<Component, (int)COMPONENT_TYPE::END>(m_Component);
}

void GameObject::Init()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		m_Component[_comp] = nullptr;
	}
}

void GameObject::Tick()
{
	for (UINT _comp=0;_comp <(UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
		{
			m_Component[_comp]->Tick();
		}
	}
}

void GameObject::Render()
{
	Renderer* _renderer = dynamic_cast<Renderer*>(m_Renderer);
	_renderer->Render();
}