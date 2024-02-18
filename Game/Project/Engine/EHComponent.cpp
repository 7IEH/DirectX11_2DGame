#include "pch.h"
#include "EHComponent.h"

Component::Component(COMPONENT_TYPE _type)
	:m_Type(_type)
	, m_Owner(nullptr)
{
}

Component::Component(const Component& _originComp)
	: Entity(_originComp)
	, m_Type(_originComp.m_Type)
	, m_Owner(nullptr)
{
}

Component::~Component()
{
}
