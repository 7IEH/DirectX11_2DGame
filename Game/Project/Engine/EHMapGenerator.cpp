#include "pch.h"
#include "EHMapGenerator.h"

#include "EHTransform.h"
#include "EHMeshRenderer.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"

#include "EHLevel.h"

#include <random>

// random rand()%(b-a+1)+a



MapGenerator::MapGenerator()
	: m_RoomInfo{}
	, m_OpeningDir(DIR_ROOM::END)
	, m_Spawned(false)
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Awake()
{
	ThreadTask::Awake();
	RoomInfoInit();

	Update();
}



void MapGenerator::Update()
{
	while (!m_Nodequeue.empty())
	{

	}
}

void MapGenerator::Spawn()
{
	// 해당 방향 으로 문이 있음
	switch (m_OpeningDir)
	{
	case DIR_ROOM::RIGHT:
	{
		// 해당 방향으로 문이 열려있는 방을 생성
		CreateRoom(DIR_ROOM::RIGHT);
	}
	break;
	case DIR_ROOM::LEFT:
	{
		CreateRoom(DIR_ROOM::LEFT);
	}
	break;
	case DIR_ROOM::TOP:
	{
		CreateRoom(DIR_ROOM::TOP);
	}
	break;
	case DIR_ROOM::BOTTOM:
	{
		CreateRoom(DIR_ROOM::BOTTOM);
	}
	break;
	case DIR_ROOM::END:
		break;
	default:
		break;
	}
	m_Spawned = TRUE;
}

void MapGenerator::CreateRoom(DIR_ROOM _dir)
{
	//int _rand = rand() % (m_RoomInfo[(UINT)_dir].size() - 1);
	//// 해당 레벨 정보 생성
	//Level* _level = new Level;
	//_level = m_RoomInfo[(UINT)_dir][_rand];

	//Destroy 한다 나를 닿은 오브젝트가 나랑 
}

void MapGenerator::RoomInfoInit()
{
	RightRoomInit();
	LeftRoomInit();
	TopRoomInit();
	BottomRoomInit();
}

void MapGenerator::RightRoomInit()
{
	// R
	Node _temp;
	SpawnPoint _temps;
	_temps._location = Vec2(0, 1);
	_temps._opendingDoor = (UINT)DIR_ROOM::RIGHT;
	_temp._SpawnPoint.push(_temps);
	_temps._location = Vec2(-1, 0);
	_temps._opendingDoor = (UINT)DIR_ROOM::RIGHT;
	_temp._SpawnPoint.push(_temps);
	_temp._location = Vec2(0, 1);
	
	m_NodeInfo[(UINT)DIR_ROOM::RIGHT].push_back(_temp);

	// RB
	

	// RT

	// RL
}

void MapGenerator::LeftRoomInit()
{
	// L, LB, LT, RL
	m_NodeInfo[(UINT)DIR_ROOM::LEFT];
}

void MapGenerator::TopRoomInit()
{
	// T, TB, TR, TL
	m_NodeInfo[(UINT)DIR_ROOM::TOP];
}

void MapGenerator::BottomRoomInit()
{
	// B, TB, BR, BL
	m_NodeInfo[(UINT)DIR_ROOM::BOTTOM];
}

void MapGenerator::SetEntryRoom()
{
	Node entry = {};
	entry._location = Vec2(50.f, 50.f);
	// T,B,L,R
	SpawnPoint _temp;
	//_temp._location=entry._location.x+
	//entry._SpawnPoint.push();
	//visited[50][50] = 1;
	//entry._SpawnPoint.push();
	m_Nodequeue.push(entry);
}
