#include "pch.h"
#include "EHMapGenerator.h"

#include "EHTransform.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"

#include <random>

class Node
{
public:
	// 1. need left Door
	// 2. need top Door
	// 3. need right Door
	// 4. need bottom Door
	int dir;
};

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Awake()
{
	ThreadTask::Awake();
	Update();
}

void MapGenerator::Update()
{
	//if (m_RootNode->dir == 1)
	//{
	//	// 1. need left Door
	//}
	//else if (m_RootNode->dir == 1)
	//{
	//	// 2. need top Door
	//}
	//else if (m_RootNode->dir == 1)
	//{
	//	// 3. need right Door
	//}
	//else
	//{
	//	// 4. need bottom Door
	//}
}
// random rand()%(b-a+1)+a