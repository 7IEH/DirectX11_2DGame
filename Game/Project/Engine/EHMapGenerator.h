#pragma once

#include "EHThreadTask.h"


class Level;

struct SpawnPoint
{
	Vec2 _location;
	int _opendingDoor;
};

struct Node
{
	Vec2 _location;
	queue<SpawnPoint>	 _SpawnPoint;
};

class MapGenerator
	:public ThreadTask
{
private:
	Vec2					m_mapSize;
	vector<Level*>			m_RoomInfo[(UINT)DIR_ROOM::END];

	vector<Node>			m_NodeInfo[(UINT)DIR_ROOM::END];

	DIR_ROOM				m_OpeningDir;

	bool					m_Spawned;
	queue<Node>			m_Nodequeue;

public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	void Spawn();
	void CreateRoom(DIR_ROOM _dir);

	void RoomInfoInit();

	void RightRoomInit();
	void LeftRoomInit();
	void TopRoomInit();
	void BottomRoomInit();

	void SetEntryRoom();

public:
	MapGenerator();
	virtual ~MapGenerator();
};

