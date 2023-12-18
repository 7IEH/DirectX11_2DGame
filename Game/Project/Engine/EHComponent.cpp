#include "pch.h"
#include "EHComponent.h"

Component::Component(COMPONENT_TYPE _type)
	:m_Type(_type)
	, m_Owner(nullptr)
{
}

Component::~Component()
{
}
