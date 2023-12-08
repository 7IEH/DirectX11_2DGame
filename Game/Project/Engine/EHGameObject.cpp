#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"

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
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
		{
			m_Component[_comp]->Render();
		}
	}
}
