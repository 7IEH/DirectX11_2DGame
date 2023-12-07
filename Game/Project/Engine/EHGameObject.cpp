#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"

void GameObject::Init()
{
	m_Component.reserve((UINT)COMPONENT_TYPE::END);
}

void GameObject::Tick()
{
}

void GameObject::Render()
{
}
