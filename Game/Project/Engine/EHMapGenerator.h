#pragma once

#include "EHThreadTask.h"

class Node;

class MapGenerator
	:public ThreadTask
{
private:
	Vec2			m_mapSize;
	
public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	void Divide(Node* _node, int _level, int _maxLevel);
	Rect GenerateRoom(Node* _node, int _level, int _maxLevel);
	void GenerateRoad();

	void DrawRectangle(Rect _drawinfo);

public:
	MapGenerator();
	virtual ~MapGenerator();
};

