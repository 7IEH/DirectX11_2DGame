#include "pch.h"
#include "EHLevel.h"

#include "EHGameObject.h"
#include "EHLayer.h"

Level::Level()
	:m_Layers{}
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type] = new Layer;
	}
}

Level::~Level()
{
	ReleaseArray(m_Layers);
}

void Level::Init()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Init();
	}
}

void Level::Tick()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Tick();
	}
}

void Level::FinalTick()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->FinalTick();
	}
}

void Level::Render()
{
	for (int _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Render();
	}
}


