#include "pch.h"
#include "EHRoomManager.h"

#include "EHRandomManager.h"
#include <EHGameObject.h>
#include <EHAssetMgr.h>
#include "EHTriggerScript.h"
#include <EHPathMgr.h>
#include <EHCollisionMgr.h>
#include <EHLevelMgr.h>
#include <EHAnimation2D.h>
#include "EHEnemyScript.h"

#include "EHGolemTurretScript.h"
#include "EHBabySlimeScript.h"
#include "EHTangleScript.h"
#include "EHGolemSoilderScript.h"
#include "EHSlimeHermitScript.h"


RoomManager::RoomManager()
	: m_vRoomRef{}
	, m_iMap{}
	, m_pPlayerObj(nullptr)
	, m_mMapRoomInfo{}
	, m_wDoorOpenAnim{}
	, m_wDoorCloseAnim{}
	, m_vCurPos{}
	, m_bBossClose(FALSE)
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::Awake()
{
	CreateRoomRef();
}

void RoomManager::Start(DungeonType _eType)
{
	switch (_eType)
	{
	case DungeonType::GOLEM:
	{
		GolemDungeonStart();
	}
	break;
	case DungeonType::None:
		break;
	default:
		break;
	}
}

void RoomManager::Update(DungeonType _eType)
{
	switch (_eType)
	{
	case DungeonType::GOLEM:
	{
		GolemDungeonUpdate();
	}
	break;
	case DungeonType::None:
		break;
	default:
		break;
	}
}

void RoomManager::CreateRoomIndividualRef(const vector<SPAWN_TYPE>& _vSpawnType, const ROOM_TYPE _eType)
{
	Room _temp;
	SpawnPoint _tempSpawn;
	for (int i = 0;i < _vSpawnType.size();i++)
	{
		_tempSpawn._SpawnPos = _vSpawnType[i];
		_temp.Spawn.push_back(_tempSpawn);
	}
	_temp.CurPos = Content::Vec2(0, 0);
	m_vRoomRef[(UINT)_eType].push_back(_temp);
}

void RoomManager::CreateRoomRef()
{
	vector<SPAWN_TYPE> _vSpawn[11];
	_vSpawn[0].push_back(SPAWN_TYPE::RIGHT);
	_vSpawn[1].push_back(SPAWN_TYPE::LEFT);
	_vSpawn[2].push_back(SPAWN_TYPE::TOP);
	_vSpawn[3].push_back(SPAWN_TYPE::BOTTOM);

	_vSpawn[4].push_back(SPAWN_TYPE::LEFT);
	_vSpawn[4].push_back(SPAWN_TYPE::TOP);

	_vSpawn[5].push_back(SPAWN_TYPE::RIGHT);
	_vSpawn[5].push_back(SPAWN_TYPE::TOP);

	_vSpawn[6].push_back(SPAWN_TYPE::BOTTOM);
	_vSpawn[6].push_back(SPAWN_TYPE::LEFT);

	_vSpawn[7].push_back(SPAWN_TYPE::BOTTOM);
	_vSpawn[7].push_back(SPAWN_TYPE::RIGHT);

	_vSpawn[8].push_back(SPAWN_TYPE::LEFT);
	_vSpawn[8].push_back(SPAWN_TYPE::RIGHT);

	_vSpawn[9].push_back(SPAWN_TYPE::TOP);
	_vSpawn[9].push_back(SPAWN_TYPE::BOTTOM);

	_vSpawn[10].push_back(SPAWN_TYPE::LEFT);
	_vSpawn[10].push_back(SPAWN_TYPE::RIGHT);
	_vSpawn[10].push_back(SPAWN_TYPE::TOP);
	_vSpawn[10].push_back(SPAWN_TYPE::BOTTOM);

	for (UINT i = 1;i < (UINT)ROOM_TYPE::END;i++)
	{
		vector<SPAWN_TYPE>_vSpawnType;
		for (int j = 0;j < _vSpawn[i - 1].size();j++)
		{
			_vSpawnType.push_back(_vSpawn[i - 1][j]);
		}

		CreateRoomIndividualRef(_vSpawnType, ROOM_TYPE(i));
		_vSpawnType.clear();
	}
}

void RoomManager::GolemDungeonStart()
{
	queue<Room>q;

	m_vCurPos = Content::Vec2(25, 25);
	GameObject* _object = new GameObject;
	Transform* _pTr = _object->AddComponent<Transform>();
	_pTr->SetRelativePosition(Vec4(0.f, 0.f, 9000.f, 0.f));
	_pTr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));
	MeshRenderer* _pRender = _object->AddComponent<MeshRenderer>();

	m_iMap[25][25] = 11;

	_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

	m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0].CurPos = Content::Vec2(25, 25);
	m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0].ParentPos = Content::Vec2(25, 25);
	m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0]._This = _object;
	m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0].Type = ROOM_TYPE::ENTRY_ROOM;
	q.push(m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0]);

	wstring _key = {};
	_key = std::to_wstring(25) + L"_" + std::to_wstring(25);

	m_mMapRoomInfo.insert(std::make_pair(_key, m_vRoomRef[(UINT)ROOM_TYPE::ENTRY_ROOM][0]));
	Object::Instantiate(_object, (int)LAYER_TYPE::BACKGROUND1);

	while (!q.empty())
	{
		auto _pCurRoom = q.front(); q.pop();
		for (UINT _idx = 0;_idx < (UINT)SPAWN_TYPE::END;_idx++)
		{
			_pCurRoom._Child[_idx] = FALSE;
		}

		for (size_t i = 0;i < _pCurRoom.Spawn.size();i++)
		{
			SpawnPoint _nxt = _pCurRoom.Spawn[i];
			Room _nxtRoom = {};
			bool _bFlag = FALSE;

			if (ROOM_TYPE::ENTRY_ROOM == _pCurRoom.Type && SPAWN_TYPE::BOTTOM == _nxt._SpawnPos)
				continue;

			CreateRoomObject(_nxt._SpawnPos, _pCurRoom, _nxtRoom, _bFlag);

			if (_bFlag)
			{
				q.push(_nxtRoom);
			}
		}
	}

	CreateDoor(L"Structure_Door_Open_Tutorial_Anim", L"Structure_Door_Close_Tutorial_Anim");

	LoadPrefab();

	CreateBossRoom(L"Object_Golem_BossDoor_Open_Anim", L"Object_Golem_BossDoor_Close_Anim");
}

void RoomManager::LoadPrefab()
{
	map<wstring, Room>::iterator iter = m_mMapRoomInfo.begin();

	for (;iter != m_mMapRoomInfo.end();iter++)
	{
		Vec4 _vStandardPos = (*iter).second._This->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

		int _iRand = 0;
		wstring _wPath = PATH;
		_wPath += L"\\Assets\\MapPrefab\\";
		switch ((*iter).second.Type)
		{
		case ROOM_TYPE::RIGHT:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 5);
			_wPath += L"R\\";
		}
		break;
		case ROOM_TYPE::LEFT:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 5);
			_wPath += L"L\\";
		}
		break;
		case ROOM_TYPE::TOP:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 5);
			_wPath += L"T\\";
		}
		break;
		case ROOM_TYPE::BOTTOM:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 5);
			_wPath += L"B\\";
		}
		break;
		case ROOM_TYPE::LEFT_TOP:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 3);
			_wPath += L"LT\\";
		}
		break;
		case ROOM_TYPE::RIGHT_TOP:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 3);
			_wPath += L"RT\\";
		}
		break;
		case ROOM_TYPE::LEFT_BOTTOM:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 3);
			_wPath += L"LB\\";
		}
		break;
		case ROOM_TYPE::RIGHT_BOTTOM:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 3);
			_wPath += L"RB\\";
		}
		break;
		case ROOM_TYPE::LEFT_RIGHT:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 4);
			_wPath += L"LR\\";
		}
		break;
		case ROOM_TYPE::TOP_BOTTOM:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 4);
			_wPath += L"TB\\";
		}
		break;
		case ROOM_TYPE::ENTRY_ROOM:
		{
			_iRand = RandomManager::GetInst()->GenerateNumber(0, 2);
			_wPath += L"ENTRY\\";
		}
		break;
		case ROOM_TYPE::END:
			break;
		default:
			break;
		}
		_wPath += std::to_wstring(_iRand) + L".txt";

		std::wifstream _sStream(_wPath);

		if (_sStream.is_open())
		{
			wstring _line = {};
			int _idx = 0;
			while (std::getline(_sStream, _line))
			{
				if (0 == _idx)
				{
					(*iter).second._This->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)
						->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(_line));
				}

				// Rock 정보
				// Position
				// Scale
				if (1 == _idx)
				{
					int _ColCount = std::stoi(_line);

					for (int i = 0;i < _ColCount;i++)
					{
						GameObject* _pRock = new GameObject;
						Transform* _pTr = _pRock->AddComponent<Transform>();
						Vec4 _vPostion = {};
						Vec4 _vScale = {};

						for (int i = 0;i < 2;i++)
						{
							std::getline(_sStream, _line);
							if (0 == i)
							{
								EH::InputVector4(_line, _vPostion);
								_vStandardPos += _vPostion;
								_pTr->SetRelativePosition(_vPostion);
							}
							else if (1 == i)
							{
								EH::InputVector4(_line, _vScale);
								_pTr->SetRelativeScale(_vScale);
							}
						}

						Object::Instantiate(_pRock, (int)LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK);
					}
				}

				// Hole 정보
				// Position
				// Scale
				if (2 == _idx)
				{
					int _HoleCount = std::stoi(_line);

					//Vec4 _vStandardPos = (*iter).second._This->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

					for (int i = 0;i < _HoleCount;i++)
					{
						GameObject* _pHole = new GameObject;
						Transform* _pTr = _pHole->AddComponent<Transform>();
						_pHole->AddComponent<Collider2D>();
						Vec4 _vPostion = {};
						Vec4 _vScale = {};

						for (int i = 0;i < 2;i++)
						{
							std::getline(_sStream, _line);
							if (0 == i)
							{
								EH::InputVector4(_line, _vPostion);
								Vec4 _vTempPos = _vStandardPos;
								_vTempPos += _vPostion;
								_pTr->SetRelativePosition(_vTempPos);
							}
							else if (1 == i)
							{
								EH::InputVector4(_line, _vScale);
								_pTr->SetRelativeScale(_vScale);
							}
						}

						Object::Instantiate(_pHole, (int)LAYER_TYPE::STRUCTURE_OBSTACLE_HOLE);
					}
				}

				if (3 == _idx)
				{
					int _iMonsterCount = std::stoi(_line);
					for (int i = 0;i < _iMonsterCount;i++)
					{
						Vec4 _vPos = {};
						Vec4 _vScale = {};

						std::getline(_sStream, _line);
						int _enemyType = std::stoi(_line);

						GameObject* _pMonster = new GameObject;
						Transform* _pTr = _pMonster->AddComponent<Transform>();
						MeshRenderer* _pRender = _pMonster->AddComponent<MeshRenderer>();
						Animator2D* _pAnim = _pMonster->AddComponent<Animator2D>();
						Collider2D* _pCol = _pMonster->AddComponent<Collider2D>();

						switch (_enemyType)
						{
						case 0:
						{
							_pTr->SetRelativeScale(Vec4(640.f, 640.f, 1.f, 1.f));
							_pMonster->AddComponent<BabySlimeScript>();

							_pAnim->AddAnimation2D(L"FSM_Enemy_Slime_Baby_Walk_Anim");
						}
						break;
						case 1:
						{
							_pTr->SetRelativeScale(Vec4(640.f, 640.f, 1.f, 1.f));
							_pMonster->AddComponent<TangleScript>();

							_pAnim->AddAnimation2D(L"FSM_Enemy_Tangle_Cycle_Anim");
						}
						break;
						case 2:
						{
							_pTr->SetRelativeScale(Vec4(640.f, 640.f, 1.f, 1.f));
							_pMonster->AddComponent<GolemTurretScript>();

							_pAnim->AddAnimation2D(L"FSM_Enemy_Golem_Turret_Attack_Down_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_Golem_Turret_Attack_Up_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_Golem_Turret_Attack_Left_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_Golem_Turret_Attack_Right_Anim");
						}
						break;
						case 3:
						{
							_pTr->SetRelativeScale(Vec4(275.f, 275.f, 1.f, 1.f));
							_pMonster->AddComponent<GolemSoilderScript>();

							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Attack_Down_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Attack_Up_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Attack_Right_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Attack_Left_Anim");

							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Walk_Down_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Walk_Up_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Walk_Right_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_GolemSoilder_Walk_Left_Anim");
						}
						break;
						case 4:
						{
							_pTr->SetRelativeScale(Vec4(640.f, 640.f, 1.f, 1.f));
							_pMonster->AddComponent<SlimeHermitScript>();

							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Attack_Down_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Attack_Up_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Attack_Right_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Attack_Left_Anim");

							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Walk_Down_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Walk_Up_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Walk_Right_Anim");
							_pAnim->AddAnimation2D(L"FSM_Enemy_SlimeHermit_Walk_Left_Anim");
						}
						break;
						default:
							break;
						}

						_vStandardPos.z = 6000.f;

						std::getline(_sStream, _line);
						EH::InputVector4(_line, _vPos);

						_vPos = _vStandardPos + _vPos;
						_pTr->SetRelativePosition(_vPos);

						_pRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"EnemyMaterial")->Clone());
						_pRender->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));

						(*iter).second._pMonster.push_back(_pMonster);

						LevelMgr::GetInst()->GetCurLevel()->AddObject(_pMonster, LAYER_TYPE::MONSTER);
					}
				}
				_idx++;
			}
		}
	}
}

void RoomManager::CreateRoomObject(SPAWN_TYPE _eType, OUT Room& _sRoom, OUT Room& _sNxtRoom, OUT bool& _bFlag)
{
	auto _vMapPos = _sRoom.CurPos;
	auto _vParentPos = _sRoom.ParentPos;

	int _iDirX[4] = { 1,-1,0,0 };
	int _iDirY[4] = { 0,0,1,-1 };

	_sNxtRoom.CurPos = _sRoom.CurPos;

	_sNxtRoom.CurPos.x += _iDirX[(UINT)_eType];
	_sNxtRoom.CurPos.y += _iDirY[(UINT)_eType];

	wstring _key = {};
	_key = std::to_wstring(_sNxtRoom.CurPos.y) + L"_" + std::to_wstring(_sNxtRoom.CurPos.x);

	if (m_iMap[_sNxtRoom.CurPos.y][_sNxtRoom.CurPos.x] != (int)ROOM_TYPE::NO_ROOM)
	{
		if (_sNxtRoom.CurPos.y == _vParentPos.y && _sNxtRoom.CurPos.x == _vParentPos.x)
			return;

		// 현재 지정한 곳에 맵이 이미 있는 경우
		// 현재 위치 타입 바꾸기
		if (_eType == SPAWN_TYPE::RIGHT)
		{
			if (m_iMap[_vMapPos.y][_vMapPos.y] == (UINT)ROOM_TYPE::RIGHT_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::RIGHT_TOP)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::TOP;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::LEFT;
			}
		}
		else if (_eType == SPAWN_TYPE::LEFT)
		{
			if (m_iMap[_vMapPos.y][_vMapPos.y] == (UINT)ROOM_TYPE::LEFT_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::LEFT_TOP)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::TOP;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::RIGHT;
			}
		}
		else if (_eType == SPAWN_TYPE::BOTTOM)
		{
			if (m_iMap[_vMapPos.y][_vMapPos.y] == (UINT)ROOM_TYPE::RIGHT_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::RIGHT;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::LEFT_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::LEFT;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::TOP_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::TOP;
			}
		}
		else
		{
			if (m_iMap[_vMapPos.y][_vMapPos.y] == (UINT)ROOM_TYPE::RIGHT_TOP)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::RIGHT;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::LEFT_TOP)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::LEFT;
			}
			else if (m_iMap[_vMapPos.y][_vMapPos.x] == (UINT)ROOM_TYPE::TOP_BOTTOM)
			{
				m_iMap[_vMapPos.y][_vMapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
			}
		}
	}
	else
	{
		_bFlag = TRUE;
		auto _iRand = RandomManager::GetInst()->GenerateNumber(0, 4);

		if (_eType == SPAWN_TYPE::BOTTOM)
		{
			if (_iRand == 0)
			{
				_sNxtRoom.Type = ROOM_TYPE::TOP;
			}
			else if (_iRand == 1)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_TOP;
			}
			else if (_iRand == 2)
			{
				_sNxtRoom.Type = ROOM_TYPE::RIGHT_TOP;
			}
			else
			{
				_sNxtRoom.Type = ROOM_TYPE::TOP_BOTTOM;
			}
		}
		else if (_eType == SPAWN_TYPE::TOP)
		{
			if (_iRand == 0)
			{
				_sNxtRoom.Type = ROOM_TYPE::BOTTOM;
			}
			else if (_iRand == 1)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_BOTTOM;
			}
			else if (_iRand == 2)
			{
				_sNxtRoom.Type = ROOM_TYPE::RIGHT_BOTTOM;
			}
			else
			{
				_sNxtRoom.Type = ROOM_TYPE::TOP_BOTTOM;
			}
		}
		else if (_eType == SPAWN_TYPE::LEFT)
		{
			if (_iRand == 0)
			{
				_sNxtRoom.Type = ROOM_TYPE::RIGHT;
			}
			else if (_iRand == 1)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_RIGHT;
			}
			else if (_iRand == 2)
			{
				_sNxtRoom.Type = ROOM_TYPE::RIGHT_TOP;
			}
			else
			{
				_sNxtRoom.Type = ROOM_TYPE::RIGHT_BOTTOM;
			}
		}
		else
		{
			if (_iRand == 0)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT;
			}
			else if (_iRand == 1)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_TOP;
			}
			else if (_iRand == 2)
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_BOTTOM;
			}
			else
			{
				_sNxtRoom.Type = ROOM_TYPE::LEFT_RIGHT;
			}
		}

		m_iMap[_sNxtRoom.CurPos.y][_sNxtRoom.CurPos.x] = (UINT)_sNxtRoom.Type;
		_sNxtRoom.ParentPos = _vParentPos;

		// 나중에 reference 늘리기
		_sNxtRoom.Spawn = m_vRoomRef[(UINT)_sNxtRoom.Type][0].Spawn;

		// 해당 방 생성
		GameObject* _obj = new GameObject;
		Transform* _tr = _obj->AddComponent<Transform>();
		MeshRenderer* _render = _obj->AddComponent<MeshRenderer>();

		_tr->SetRelativePosition(Vec4(1600.f * (_sNxtRoom.CurPos.x - 25.f),
			900.f * (25.f - _sNxtRoom.CurPos.y), 9000.f, 1.f));
		_tr->SetRelativeScale(Vec4(1600.f, 900.f, 1.f, 1.f));

		_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));

		_sNxtRoom._This = _obj;
		_sNxtRoom.ParentPos = _sRoom.CurPos;

		if (SPAWN_TYPE::RIGHT == _eType)
		{
			_sNxtRoom.m_ParentDir = SPAWN_TYPE::LEFT;
			_sRoom._Child[(UINT)SPAWN_TYPE::LEFT] = _obj;
			_sNxtRoom._Parent = _sRoom._This;
		}
		else if (SPAWN_TYPE::LEFT == _eType)
		{
			_sNxtRoom.m_ParentDir = SPAWN_TYPE::RIGHT;
			_sRoom._Child[(UINT)SPAWN_TYPE::RIGHT] = _obj;
			_sNxtRoom._Parent = _sRoom._This;
		}
		else if (SPAWN_TYPE::BOTTOM == _eType)
		{
			_sNxtRoom.m_ParentDir = SPAWN_TYPE::TOP;
			_sRoom._Child[(UINT)SPAWN_TYPE::TOP] = _obj;
			_sNxtRoom._Parent = _sRoom._This;
		}
		else
		{
			_sNxtRoom.m_ParentDir = SPAWN_TYPE::BOTTOM;
			_sRoom._Child[(UINT)SPAWN_TYPE::BOTTOM] = _obj;
			_sNxtRoom._Parent = _sRoom._This;
		}

		m_mMapRoomInfo.insert(std::make_pair(_key, _sNxtRoom));
		Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND1);
	}
}

void RoomManager::CreateDoor(const wstring& _wOpenAnim, const wstring& _wCloseAnim)
{
	map<wstring, Room>::iterator iter = m_mMapRoomInfo.begin();

	for (;iter != m_mMapRoomInfo.end();iter++)
	{
		Room _sRoom = (*iter).second;
		Vec4 _vCurPos = _sRoom._This->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

		for (size_t j = 0; j < _sRoom.Spawn.size();j++)
		{
			Vec4 _vStandardPos = _vCurPos;
			_vStandardPos.z = 8000.f;
			Vec3 _vRotation = Vec3(0.f, 0.f, 0.f);

			switch (SPAWN_TYPE(_sRoom.Spawn[j]._SpawnPos))
			{
			case SPAWN_TYPE::RIGHT:
			{
				_vStandardPos.x += 650.f;
				_vRotation = Vec3(0.f, 0.f, -90.f);
			}
			break;
			case SPAWN_TYPE::LEFT:
			{
				_vStandardPos.x -= 650.f;
				_vRotation = Vec3(0.f, 0.f, 90.f);
			}
			break;
			case SPAWN_TYPE::BOTTOM:
			{
				_vStandardPos.y -= 380.f;
				_vRotation = Vec3(0.f, 0.f, 180.f);
			}
			break;
			case SPAWN_TYPE::TOP:
			{
				_vStandardPos.y += 380.f;
			}
			break;
			case SPAWN_TYPE::END:
				break;
			default:
				break;
			}

			GameObject* _pDoor = new GameObject;
			if (ROOM_TYPE::ENTRY_ROOM == _sRoom.Type && SPAWN_TYPE::BOTTOM == _sRoom.Spawn[j]._SpawnPos)
			{
				CreateDoorIndividual(_pDoor, _vStandardPos, _vRotation, _wOpenAnim, _wCloseAnim, TRUE, FALSE);
			}
			else
			{
				CreateDoorIndividual(_pDoor, _vStandardPos, _vRotation, _wOpenAnim, _wCloseAnim, FALSE, FALSE);
			}

			(*iter).second._pDoor.push_back(_pDoor);
		}
	}
}

void RoomManager::GolemDungeonUpdate()
{
	// DungeonUpdte
	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TRIGGER);

	wstring _key = {};
	_key = std::to_wstring(m_vCurPos.y) + L"_" + std::to_wstring(m_vCurPos.x);
	map<wstring, Room>::iterator iter = m_mMapRoomInfo.find(_key);

	if (iter == m_mMapRoomInfo.end())
		return;

	Room _CurRoom = (*iter).second;
	bool _flag = FALSE;

	for (size_t i = 0;i < _CurRoom._pMonster.size();i++)
	{
		if (nullptr != _CurRoom._pMonster[i])
		{
			if (!_CurRoom._pMonster[i]->GetScript<EnemyScript>()->GetChase())
			{
				_CurRoom._pMonster[i]->GetScript<EnemyScript>()->SetChase(TRUE);
			}
		}
	}

	for (size_t i = 0;i < _CurRoom._pMonster.size();i++)
	{
		if (!_CurRoom._pMonster[i]->GetDead())
		{
			_flag = TRUE;
		}
	}

	if (!_flag)
	{
		for (size_t i = 0;i < _CurRoom._pDoor.size();i++)
		{
			if ((L"Object_Door" == _CurRoom._pDoor[i]->GetName()) &&
				m_wDoorOpenAnim != _CurRoom._pDoor[i]
				->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
			{
				_CurRoom._pDoor[i]
					->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(m_wDoorOpenAnim, FALSE);

				_CurRoom._pDoor[i]
					->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
			}

			if ((L"Object_Boss_Door" == _CurRoom._pDoor[i]->GetName()) &&
				L"Object_Golem_BossDoor_Open_Anim" != _CurRoom._pDoor[i]
				->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName() && !m_bBossClose)
			{
				_CurRoom._pDoor[i]
					->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Golem_BossDoor_Open_Anim", FALSE);

				_CurRoom._pDoor[i]
					->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
			}
		}
	}
	else
	{
		for (size_t i = 0;i < _CurRoom._pDoor.size();i++)
		{
			if ((L"Object_Door" == _CurRoom._pDoor[i]->GetName()) &&
				m_wDoorCloseAnim != _CurRoom._pDoor[i]
				->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
			{
				_CurRoom._pDoor[i]
					->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(m_wDoorCloseAnim, FALSE);

				_CurRoom._pDoor[i]
					->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}

			if ((L"Object_Boss_Door" == _CurRoom._pDoor[i]->GetName()) &&
				L"Object_Golem_BossDoor_Close_Anim" != _CurRoom._pDoor[i]
				->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
			{
				_CurRoom._pDoor[i]
					->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Golem_BossDoor_Close_Anim", FALSE);

				_CurRoom._pDoor[i]
					->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			}
		}
	}
}

void RoomManager::CreateBossRoom(const wstring& _wOpenAnim, const wstring& _wCloseAnim)
{
	queue<std::pair<int, int>>_room = {};
	int _bVisited[50][50] = {};

	_room.push(std::make_pair(25, 25));

	int _bFlag = FALSE;

	_bVisited[25][25] = 1;

	int _dirX[4] = { 1,-1,0,0 };
	int _dirY[4] = { 0,0,-1,1 };

	auto _iBRoomNum = 0;

	while (!_room.empty())
	{
		auto _curPos = _room.front(); _room.pop();
		for (int _dir = 0;_dir < 4;_dir++)
		{
			if (_bFlag)
				break;

			auto _nxtPosX = _curPos.first + _dirX[_dir];
			auto _nxtPosY = _curPos.second + _dirY[_dir];

			if (_nxtPosX < 0 || _nxtPosY < 0 || _nxtPosX >= 50 || _nxtPosY >= 50)continue;
			if (_bVisited[_nxtPosY][_nxtPosX] || !m_iMap[_nxtPosY][_nxtPosX])continue;

			_bVisited[_nxtPosY][_nxtPosX] = _bVisited[_curPos.second][_curPos.first] + 1;

			if (_iBRoomNum < _bVisited[_nxtPosY][_nxtPosX])
			{
				_iBRoomNum = _bVisited[_nxtPosY][_nxtPosX];
			}

			_room.push(std::make_pair(_nxtPosX, _nxtPosY));
		}
	}

	vector<std::pair<int, int>>_v;

	for (int y = 0;y < 50;y++)
	{
		for (int x = 0;x < 50;x++)
		{
			if (_bVisited[y][x] >= _iBRoomNum - 1)
			{
				_v.push_back(std::make_pair(y, x));
			}
		}
	}

	auto _bRoomidx = RandomManager::GetInst()->GenerateNumber(0, _v.size() - 1);

	int _iBossMapY = _v[_bRoomidx].first;
	int _iBossMapX = _v[_bRoomidx].second;

	SPAWN_TYPE _eType = SPAWN_TYPE::END;

	switch (ROOM_TYPE(m_iMap[_iBossMapY][_iBossMapX]))
	{
	case ROOM_TYPE::RIGHT:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 2);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else if (1 == _iRand)
		{
			_eType = SPAWN_TYPE::TOP;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::LEFT:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 2);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
		else if (1 == _iRand)
		{
			_eType = SPAWN_TYPE::TOP;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::TOP:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 2);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else if (1 == _iRand)
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::BOTTOM:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 2);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else if (1 == _iRand)
		{
			_eType = SPAWN_TYPE::TOP;
		}
		else
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
	}
	break;
	case ROOM_TYPE::LEFT_TOP:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::RIGHT_TOP:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::LEFT_BOTTOM:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
		else
		{
			_eType = SPAWN_TYPE::TOP;
		}
	}
	break;
	case ROOM_TYPE::RIGHT_BOTTOM:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else
		{
			_eType = SPAWN_TYPE::TOP;
		}
	}
	break;
	case ROOM_TYPE::LEFT_RIGHT:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::TOP;
		}
		else
		{
			_eType = SPAWN_TYPE::BOTTOM;
		}
	}
	break;
	case ROOM_TYPE::TOP_BOTTOM:
	{
		int _iRand = RandomManager::GetInst()->GenerateNumber(0, 1);

		if (0 == _iRand)
		{
			_eType = SPAWN_TYPE::LEFT;
		}
		else
		{
			_eType = SPAWN_TYPE::RIGHT;
		}
	}
	break;
	case ROOM_TYPE::END:
		break;
	default:
		break;
	}

	Vec4 _vPos = Vec4(1600.f * (_iBossMapX - 25.f), 900.f * (25.f - _iBossMapY), 100.f, 0.f);
	Vec3 _vRot = Vec3(0.f, 0.f, 0.f);

	switch (_eType)
	{
	case SPAWN_TYPE::RIGHT:
	{
		_vPos.x += 650.f;
		_vRot = Vec3(0.f, 0.f, -90.f);
	}
	break;
	case SPAWN_TYPE::LEFT:
	{
		_vPos.x -= 650.f;
		_vRot = Vec3(0.f, 0.f, 90.f);
	}
	break;
	case SPAWN_TYPE::BOTTOM:
	{
		_vPos.y -= 380.f;
		_vRot = Vec3(0.f, 0.f, 180.f);
	}
	break;
	case SPAWN_TYPE::TOP:
	{
		_vPos.y += 380.f;
	}
	break;
	case SPAWN_TYPE::END:
		break;
	default:
		break;
	}

	GameObject* _pDoor = new GameObject;

	CreateDoorIndividual(_pDoor, _vPos, _vRot, _wOpenAnim, _wCloseAnim, FALSE, TRUE, TRIGGER_TYPE::GOLEM_BOSS_MOVE_TRIGGER);

	wstring _key = {};
	_key = std::to_wstring(_iBossMapY) + L"_" + std::to_wstring(_iBossMapX);
	map<wstring, Room>::iterator iter = m_mMapRoomInfo.find(_key);

	(*iter).second._pDoor.push_back(_pDoor);
}

void RoomManager::CreateDoorIndividual(GameObject* _pDoor, const Vec4& _vPos, const Vec3& _vRot, const wstring& _wOpenAnim, const wstring& _wCloseAnim, bool _bEntryDoor, bool _bBossDoor, TRIGGER_TYPE _eType)
{
	if (_bEntryDoor)
	{
		_pDoor->SetName(L"Object_Intro_Door");
	}
	else if (_bBossDoor)
	{
		_pDoor->SetName(L"Object_Boss_Door");
	}
	else
	{
		_pDoor->SetName(L"Object_Door");
	}

	if (!_bEntryDoor)
	{
		// Collider 설정
		Collider2D* _pCol = _pDoor->AddComponent<Collider2D>();
		_pCol->Enabled(FALSE);
	}
	// Renderer 설정
	MeshRenderer* _pRenderer = _pDoor->AddComponent<MeshRenderer>();
	_pRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_pRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));

	if (!_bEntryDoor)
	{
		// Script 설정
		TriggerScript* _pTrigger = _pDoor->AddComponent<TriggerScript>();
		_pTrigger->SetTriggerType(_eType);
	}

	// Animator2D 설정
	Animator2D* _pAnimator = _pDoor->AddComponent<Animator2D>();
	_pAnimator->AddAnimation2D(_wOpenAnim);
	_pAnimator->AddAnimation2D(_wCloseAnim);

	if (!_bBossDoor)
	{
		m_wDoorOpenAnim = _wOpenAnim;
		m_wDoorCloseAnim = _wCloseAnim;
	}

	_pAnimator->Play(_wCloseAnim, FALSE);

	// Transform 설정
	Transform* _pDoorTr = _pDoor->AddComponent<Transform>();
	float _width = 275.f;
	float _height = 160.f;

	Vec4 _vStandardPos = _vPos;
	_vStandardPos.z = 8000.f;
	_pDoorTr->SetRelativeScale(Vec4(_width, _height, 1.f, 1.f));
	_pDoorTr->SetRelativePosition(_vStandardPos);
	_pDoorTr->SetRelativeRotation(_vRot);

	LevelMgr::GetInst()->GetCurLevel()->AddObject(_pDoor, LAYER_TYPE::TRIGGER);
}