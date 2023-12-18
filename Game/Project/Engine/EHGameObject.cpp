#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"
#include "EHScript.h"

GameObject::GameObject()
	:m_Component{}
	,m_Renderer(nullptr)
	,m_vScripts{}
{
}

GameObject::~GameObject()
{
	ReleaseArray<Component, (int)COMPONENT_TYPE::END>(m_Component);
}

void GameObject::Init()
{
	/*for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if(m_Component[_comp]==nullptr)
			m_Component[_comp] = nullptr;
	}*/
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

	if (!m_vScripts.empty())
	{
		for (Script* _script : m_vScripts)
		{
			_script->Tick();
		}
	}
}

void GameObject::FinalTick()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
		{
			m_Component[_comp]->FinalTick();
		}
	}
}

void GameObject::Render()
{
	Renderer* _renderer = dynamic_cast<Renderer*>(m_Renderer);
	if(_renderer!=nullptr)
		_renderer->Render();
}