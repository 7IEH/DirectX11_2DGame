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
		m_Layers[_type]->m_Type = LAYER_TYPE(_type);
	}
}

Level::~Level()
{
	ReleaseArray(m_Layers);
}

void Level::Awake()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Awake();
	}
}

void Level::Start()
{

}

void Level::Update()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Update();
	}
}

void Level::FixedUpdate()
{

}

void Level::LateUpdate()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->LateUpdate();
	}
}

void Level::Clear()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_Layers[i]->m_GameObjects.clear();
	}
}


