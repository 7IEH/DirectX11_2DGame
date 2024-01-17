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
	_temp.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT].push_back(_temp);

	// Left
	Room _temp2;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp2.Spawn.push_back(_tempSpawn);
	_temp2.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT].push_back(_temp2);

	// Top
	Room _temp3;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp3.Spawn.push_back(_tempSpawn);
	_temp3.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::TOP].push_back(_temp3);

	// Bottom
	Room _temp4;
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp4.Spawn.push_back(_tempSpawn);
	_temp4.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::BOTTOM].push_back(_temp4);

	// ROOM_TYPE::LEFT_TOP:
	Room _temp5;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp5.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp5.Spawn.push_back(_tempSpawn);
	_temp5.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_TOP].push_back(_temp5);

	// ROOM_TYPE::RIGHT_TOP:
	Room _temp6;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp6.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp6.Spawn.push_back(_tempSpawn);
	_temp6.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT_TOP].push_back(_temp6);

	// ROOM_TYPE::LEFT_BOTTOM:
	Room _temp7;
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp7.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp7.Spawn.push_back(_tempSpawn);
	_temp7.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_BOTTOM].push_back(_temp7);

	// ROOM_TYPE::RIGHT_BOTTOM:
	Room _temp8;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp8.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp8.Spawn.push_back(_tempSpawn);
	_temp8.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::RIGHT_BOTTOM].push_back(_temp8);

	// ROOM_TYPE::LEFT_RIGHT:
	Room _temp9;
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_temp9.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_temp9.Spawn.push_back(_tempSpawn);
	_temp9.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::LEFT_RIGHT].push_back(_temp9);

	// ROOM_TYPE::TOP_BOTTOM:
	Room _temp10;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_temp10.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_temp10.Spawn.push_back(_tempSpawn);
	_temp10.CurPos = Content::Vec2(0, 0);
	m_RoomRef[(UINT)ROOM_TYPE::TOP_BOTTOM].push_back(_temp10);
}

void RoomManager::AllRoomSetting(vector<Room>& _mapInfo)
{
	vector<Room>::iterator iter = _mapInfo.begin();

	for (;iter != _mapInfo.end();iter++)
	{
		// 내 룸 정보 collider 기반으로 
		// parent 룸 정보로 이동하게 끔 만들어야 함
		// trigger script 생성
		GameObject* _obj = (*iter)._This;
		Room _room = (*iter);

		if (_room._Parent == nullptr)
		{
			// Entry Room
		}
		else
		{
			// Room
		}
	}
}

void RoomManager::CreateRightRoom(GameObject* _backGround)
{
	
}

void RoomManager::CreateLeftRoom(GameObject* _backGround)
{

}

void RoomManager::CreateTopRoom(GameObject* _backGround)
{

}

void RoomManager::CreateBottomRoom(GameObject* _backGround)
{

}

void RoomManager::CreateLeftTopRoom(GameObject* _backGround)
{

}

void RoomManager::CreateRightTopRoom(GameObject* _backGround)
{

}

void RoomManager::CreateLeftBottomRoom(GameObject* _backGround)
{

}

void RoomManager::CreateRightBottomRoom(GameObject* _backGround)
{

}

void RoomManager::CreateLeftRightRoom(GameObject* _backGround)
{
}

void RoomManager::CreateTopBottomRoom(GameObject* _backGround)
{
}
