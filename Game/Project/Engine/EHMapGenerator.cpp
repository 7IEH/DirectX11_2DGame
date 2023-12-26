#include "pch.h"
#include "EHMapGenerator.h"

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
	Divide();
}

void MapGenerator::Divide()
{
}