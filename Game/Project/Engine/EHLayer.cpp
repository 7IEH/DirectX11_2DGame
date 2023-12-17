#include "pch.h"
#include "EHLayer.h"

#include "EHGameObject.h"

Layer::Layer()
	:m_GameObjects{}
{
}

Layer::~Layer()
{
	ReleaseVector(m_GameObjects);
}


void Layer::Init()
{
	if (!m_GameObjects.empty())
	{
		for (GameObject* _obj : m_GameObjects)
		{
			if (_obj != nullptr)
				_obj->Init();
		}
	}
}

void Layer::Tick()
{
	if (!m_GameObjects.empty())
	{
		for (GameObject* _obj : m_GameObjects)
		{
			if (_obj != nullptr)
				_obj->Tick();
		}
	}
}

void Layer::FinalTick()
{
	if (!m_GameObjects.empty())
	{
		for (GameObject* _obj : m_GameObjects)
		{
			if (_obj != nullptr)
				_obj->FinalTick();
		}
	}
}

void Layer::Render()
{
	if (!m_GameObjects.empty())
	{
		for (GameObject* _obj : m_GameObjects)
		{
			if (_obj != nullptr)
				_obj->Render();
		}
	}
}