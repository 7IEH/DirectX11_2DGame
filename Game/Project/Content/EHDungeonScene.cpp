#include "pch.h"
#include "EHDungeonScene.h"

#include "EHLevelMgr.h"
#include "EHAssetMgr.h"

#include "EHGameObject.h"

#include "EHRoomManager.h"

void DungeonScene::Awake()
{
	srand(time(NULL));
	/*********************
	|	Main_Camera
	*********************/
	GameObject* _MainCamera = new GameObject;
	Transform* _tr = _MainCamera->AddComponent<Transform>();
	Camera* _camera = _MainCamera->AddComponent<Camera>();
	CameraScript* _cameraScript = _MainCamera->AddComponent<CameraScript>();

	_camera->AllVisibleSet(TRUE);
	_camera->LayerVisibleSet(LAYER_TYPE::BACKGROUND, TRUE);
	_camera->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);

	_tr->SetRelativeScale(Vec4(1.f, 1.f, 1.f, 1.f));
	_tr->SetRelativePosition(Vec4(0.f, 0.f, -10.f, 1.f));
	_tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	Object::Instantiate(_MainCamera, (int)LAYER_TYPE::CAMERA);
	/***********************
	|	RoomManger Awake
	***********************/

	RoomManager::GetInst()->Awake();

	for (int i = 1;i < (UINT)ROOM_TYPE::END;i++)
	{
		m_RoomRef[i] = RoomManager::GetInst()->GetRoomRef(ROOM_TYPE(i));
	}

	SpawnPoint _tempSpawn;

	Room _entry;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_entry.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_entry.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_entry.Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_entry.Spawn.push_back(_tempSpawn);
	_entry.CurRoom = Content::Vec2(25.f, 25.f);

	m_MapPos[25][25] = 4;

	GameObject* _EntryRoom = new GameObject;
	_tr = _EntryRoom->AddComponent<Transform>();
	MeshRenderer* _render = _EntryRoom->AddComponent<MeshRenderer>();

	_tr->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 1.f));
	_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

	_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

	Object::Instantiate(_EntryRoom, (int)LAYER_TYPE::BACKGROUND);

	_entry._This = _EntryRoom;

	queue<Room>q;
	q.push(_entry);
	m_MapInfo.push_back(_EntryRoom);

	ROOM_TYPE _type;

	while (!q.empty())
	{
		Room _CurRoom = q.front(); q.pop();
		Content::Vec2 _mapPos = _CurRoom.CurRoom;
		Content::Vec2 _parentPos = _CurRoom.ParentRoom;

		for (int i = 0;i < _CurRoom.Spawn.size();i++)
		{
			GameObject* _obj;
			SpawnPoint _nxt = _CurRoom.Spawn[i];
			Room _nxtRoom;
			Content::Vec2 _nxtPos = {};
			int _rand = rand() % 4;
			switch (_nxt._SpawnPos)
			{
			case SPAWN_TYPE::RIGHT:
				_nxtPos = _CurRoom.CurRoom;
				_nxtPos.x += 1;
				if (m_Map[_nxtPos.y][_nxtPos.x] != 0)
				{
					if (_nxtPos.y == _parentPos.y && _nxtPos.x == _parentPos.x)
						continue;
					// 현재 방에 타입을 바꿔야함
					// 가지고 있는 룸타입 가져와서 바꿔야함
					if (m_Map[_mapPos.y][_mapPos.y] == (UINT)ROOM_TYPE::RIGHT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::RIGHT_TOP)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::TOP;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::LEFT;
					}
					continue;
				}
				// 랜덤

				if (_rand == 0)
				{
					_type = ROOM_TYPE::LEFT;
				}
				else if (_rand == 1)
				{
					_type = ROOM_TYPE::LEFT_BOTTOM;
				}
				else if (_rand == 2)
				{
					_type = ROOM_TYPE::LEFT_RIGHT;
				}
				else
				{
					_type = ROOM_TYPE::LEFT_TOP;
				}

				m_Map[_nxtPos.y][_nxtPos.x] = (UINT)_type;
				_nxtRoom.ParentRoom = _mapPos;
				_nxtRoom.CurRoom = _nxtPos;
				_nxtRoom.Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f*(25.f-_nxtPos.y), 900.f*(25.f-_nxtPos.x), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom._This = _obj;
				_nxtRoom.ParentRoom = _CurRoom.ParentRoom;

				m_MapInfo.push_back(_obj);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::LEFT:
				_nxtPos = _CurRoom.CurRoom;
				_nxtPos.x -= 1;
				if (m_Map[_nxtPos.y][_nxtPos.x] != 0)
				{
					if (_nxtPos.y == _parentPos.y && _nxtPos.x == _parentPos.x)
						continue;
					// 현재 방에 타입을 바꿔야함
					// 가지고 있는 룸타입 가져와서 바꿔야함
					if (m_Map[_mapPos.y][_mapPos.y] == (UINT)ROOM_TYPE::LEFT_TOP)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::TOP;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::RIGHT;
					}
					continue;
				}
				// 랜덤

				if (_rand == 0)
				{
					_type = ROOM_TYPE::RIGHT;
				}
				else if (_rand == 1)
				{
					_type = ROOM_TYPE::RIGHT_BOTTOM;
				}
				else if (_rand == 2)
				{
					_type = ROOM_TYPE::LEFT_RIGHT;
				}
				else
				{
					_type = ROOM_TYPE::RIGHT_TOP;
				}


				m_Map[_nxtPos.y][_nxtPos.x] = (UINT)_type;
				_nxtRoom.ParentRoom = _mapPos;
				_nxtRoom.CurRoom = _nxtPos;
				_nxtRoom.Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.y), 900.f * (25.f - _nxtPos.x), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom._This = _obj;
				_nxtRoom.ParentRoom = _CurRoom.ParentRoom;

				m_MapInfo.push_back(_obj);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::BOTTOM:
				_nxtPos = _CurRoom.CurRoom;
				_nxtPos.y += 1;
				if (m_Map[_nxtPos.y][_nxtPos.x] != 0)
				{
					if (_nxtPos.y == _parentPos.y && _nxtPos.x == _parentPos.x)
						continue;
					// 현재 방에 타입을 바꿔야함
					// 가지고 있는 룸타입 가져와서 바꿔야함
					if (m_Map[_mapPos.y][_mapPos.y] == (UINT)ROOM_TYPE::LEFT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::LEFT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::RIGHT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::RIGHT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::TOP_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::TOP;
					}
					continue;
				}
				else if (m_Map[_nxtPos.y][_nxtPos.x] == m_Map[_mapPos.y][_mapPos.x])
					continue;
				// 랜덤

				if (_rand == 0)
				{
					_type = ROOM_TYPE::TOP;
				}
				else if (_rand == 1)
				{
					_type = ROOM_TYPE::LEFT_TOP;
				}
				else if (_rand == 2)
				{
					_type = ROOM_TYPE::RIGHT_TOP;
				}
				else
				{
					_type = ROOM_TYPE::TOP_BOTTOM;
				}


				m_Map[_nxtPos.y][_nxtPos.x] = (UINT)_type;
				_nxtRoom.ParentRoom = _mapPos;
				_nxtRoom.CurRoom = _nxtPos;
				_nxtRoom.Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.y), 900.f * (25.f - _nxtPos.x), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom._This = _obj;
				_nxtRoom.ParentRoom = _CurRoom.ParentRoom;

				m_MapInfo.push_back(_obj);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::TOP:
				_nxtPos = _CurRoom.CurRoom;
				_nxtPos.y -= 1;
				if (m_Map[_nxtPos.y][_nxtPos.x] != 0)
				{
					if (_nxtPos.y == _parentPos.y && _nxtPos.x == _parentPos.x)
						continue;
					// 현재 방에 타입을 바꿔야함
					// 가지고 있는 룸타입 가져와서 바꿔야함
					if (m_Map[_mapPos.y][_mapPos.y] == (UINT)ROOM_TYPE::LEFT_TOP)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::LEFT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::RIGHT_TOP)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::RIGHT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::TOP_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
					}
					continue;
				}
				// 랜덤
				if (_rand == 0)
				{
					_type = ROOM_TYPE::BOTTOM;
				}
				else if (_rand == 1)
				{
					_type = ROOM_TYPE::LEFT_BOTTOM;
				}
				else if (_rand == 2)
				{
					_type = ROOM_TYPE::RIGHT_BOTTOM;
				}
				else
				{
					_type = ROOM_TYPE::TOP_BOTTOM;
				}


				m_Map[_nxtPos.y][_nxtPos.x] = (UINT)_type;
				_nxtRoom.ParentRoom = _mapPos;
				_nxtRoom.CurRoom = _nxtPos;
				_nxtRoom.Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.y), 900.f * (25.f - _nxtPos.x), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom._This = _obj;
				_nxtRoom.ParentRoom = _CurRoom.ParentRoom;

				m_MapInfo.push_back(_obj);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::END:
				break;
			default:
				break;
			}
		}
	}

	Level::Awake();
}

void DungeonScene::Start()
{
	Level::Start();
}

void DungeonScene::Update()
{
	Level::Update();
}

void DungeonScene::FixedUpdate()
{
	Level::FixedUpdate();
}

void DungeonScene::LateUpdate()
{
	Level::LateUpdate();
}
