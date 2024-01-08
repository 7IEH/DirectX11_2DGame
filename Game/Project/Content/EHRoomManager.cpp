#include "pch.h"
#include "EHRoomManager.h"

RoomManager::RoomManager()
{

}

RoomManager::~RoomManager()
{

}

void RoomManager::Awake()
{
	// Right
	Room _temp;
	SpawnPoint _tempSpawn;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp.Spawn.push_back(_tempSpawn);
	_temp.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT].push_back(_temp);

	// Left
	Room _temp2;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp2.Spawn.push_back(_tempSpawn);
	_temp2.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT].push_back(_temp2);

	// Top
	Room _temp3;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp3.Spawn.push_back(_tempSpawn);
	_temp3.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::TOP].push_back(_temp3);

	// Bottom
	Room _temp4;
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp4.Spawn.push_back(_tempSpawn);
	_temp4.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::BOTTOM].push_back(_temp4);

	// ROOM_TYPE::LEFT_TOP:
	Room _temp5;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp5.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp5.Spawn.push_back(_tempSpawn);
	_temp5.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_TOP].push_back(_temp5);

	// ROOM_TYPE::RIGHT_TOP:
	Room _temp6;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp6.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp6.Spawn.push_back(_tempSpawn);
	_temp6.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT_TOP].push_back(_temp6);

	// ROOM_TYPE::LEFT_BOTTOM:
	Room _temp7;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp7.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp7.Spawn.push_back(_tempSpawn);
	_temp7.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_BOTTOM].push_back(_temp7);

	// ROOM_TYPE::RIGHT_BOTTOM:
	Room _temp8;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp8.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp8.Spawn.push_back(_tempSpawn);
	_temp8.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT_BOTTOM].push_back(_temp8);

	// ROOM_TYPE::LEFT_RIGHT:
	Room _temp9;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp9.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp9.Spawn.push_back(_tempSpawn);
	_temp9.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_RIGHT].push_back(_temp9);

	// ROOM_TYPE::TOP_BOTTOM:
	Room _temp10;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp10.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp10.Spawn.push_back(_tempSpawn);
	_temp10.CurRoom = Content::Vec2(0.f, 0.f);
	m_RoomRef[(UINT)ROOM_TYPE::TOP_BOTTOM].push_back(_temp10);
}