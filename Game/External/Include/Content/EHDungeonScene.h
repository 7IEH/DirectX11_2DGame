#pragma once

#include "EHLevel.h"
#include "global.h"

class DungeonScene
	:public Level
{
private:
	int						m_Map[101][101];
	vector<GameObject*>		m_MapInfo;
	GameObject*				m_EntryRoom;

	// Room Reference
	vector<Room>			m_RoomRef[(UINT)ROOM_TYPE::END];
	int						m_MapPos[50][50];

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;

private:
};

