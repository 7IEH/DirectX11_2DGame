#include "pch.h"
#include "EHDungeonScene.h"

#include "EHLevelMgr.h"
#include "EHAssetMgr.h"

#include "EHGameObject.h"

#include "EHRoomManager.h"
#include "EHCollisionMgr.h"

#include "EHLight2D.h"
#include "EHLight2DScript.h"
#include "EHPlayerScript.h"
#include "EHTriggerScript.h"

#include "EHOutLineScript.h"

DungeonScene::DungeonScene()
{
}

DungeonScene::~DungeonScene()
{
	ReleaseVector(m_MapRoomInfo);
}

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

	Room* _entry = new Room;
	_tempSpawn._SpawnPos = SPAWN_TYPE::TOP;
	_entry->Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::BOTTOM;
	_entry->Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::LEFT;
	_entry->Spawn.push_back(_tempSpawn);
	_tempSpawn._SpawnPos = SPAWN_TYPE::RIGHT;
	_entry->Spawn.push_back(_tempSpawn);
	_entry->CurPos = Content::Vec2(25.f, 25.f);

	m_MapPos[25][25] = 4;

	GameObject* _EntryRoom = new GameObject;
	_tr = _EntryRoom->AddComponent<Transform>();
	MeshRenderer* _render = _EntryRoom->AddComponent<MeshRenderer>();

	_tr->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 1.f));
	_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

	_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

	Object::Instantiate(_EntryRoom, (int)LAYER_TYPE::BACKGROUND);

	_entry->_This = _EntryRoom;
	_entry->Type = ROOM_TYPE::ENTRY_ROOM;
	_entry->_Parent = nullptr;

	queue<Room*>q;
	q.push(_entry);
	m_MapInfo.push_back(_EntryRoom);
	m_MapRoomInfo.push_back(_entry);

	ROOM_TYPE _type;

	// 랜덤 룸 생성
	while (!q.empty())
	{
		Room* _CurRoom = q.front(); q.pop();
		Content::Vec2 _mapPos = _CurRoom->CurPos;
		Content::Vec2 _parentPos = _CurRoom->ParentPos;
		for (UINT _idx = 0;_idx < (UINT)SPAWN_TYPE::END;_idx++)
		{
			_CurRoom->_Child[_idx] = nullptr;
		}

		for (int i = 0;i < _CurRoom->Spawn.size();i++)
		{
			GameObject* _obj = nullptr;
			SpawnPoint _nxt = _CurRoom->Spawn[i];
			Room* _nxtRoom = new Room;
			Content::Vec2 _nxtPos = {};
			int _rand = rand() % 4;
			switch (_nxt._SpawnPos)
			{
			case SPAWN_TYPE::RIGHT:
				_nxtPos = _CurRoom->CurPos;
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
						_CurRoom->Type = ROOM_TYPE::BOTTOM;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::RIGHT_TOP)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::TOP;
						_CurRoom->Type = ROOM_TYPE::TOP;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::LEFT;
						_CurRoom->Type = ROOM_TYPE::LEFT;
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
				_nxtRoom->ParentPos = _mapPos;
				_nxtRoom->CurPos = _nxtPos;
				_nxtRoom->Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				// 해당 방 생성
				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.x), 900.f * (25.f - _nxtPos.y), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom->_This = _obj;
				_nxtRoom->ParentPos = _CurRoom->CurPos;
				_nxtRoom->Type = _type;
				_nxtRoom->m_ParentDir = SPAWN_TYPE::RIGHT;
				_CurRoom->_Child[(UINT)SPAWN_TYPE::RIGHT] = _obj;
				_nxtRoom->_Parent = _CurRoom->_This;

				m_MapInfo.push_back(_obj);
				m_MapRoomInfo.push_back(_nxtRoom);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::LEFT:
				_nxtPos = _CurRoom->CurPos;
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
						_CurRoom->Type = ROOM_TYPE::TOP;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::BOTTOM;
						_CurRoom->Type = ROOM_TYPE::BOTTOM;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::LEFT_RIGHT)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::RIGHT;
						_CurRoom->Type = ROOM_TYPE::RIGHT;
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
				_nxtRoom->ParentPos = _mapPos;
				_nxtRoom->CurPos = _nxtPos;
				_nxtRoom->Spawn = m_RoomRef[(UINT)_type][0].Spawn;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.x), 900.f * (25.f - _nxtPos.y), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_nxtRoom->_This = _obj;
				_nxtRoom->ParentPos = _CurRoom->CurPos;
				_nxtRoom->Type = _type;
				_nxtRoom->m_ParentDir = SPAWN_TYPE::LEFT;
				_CurRoom->_Child[(UINT)SPAWN_TYPE::LEFT] = _obj;
				_nxtRoom->_Parent = _CurRoom->_This;

				m_MapInfo.push_back(_obj);
				m_MapRoomInfo.push_back(_nxtRoom);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::BOTTOM:
				_nxtPos = _CurRoom->CurPos;
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
						_CurRoom->Type = ROOM_TYPE::LEFT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::RIGHT_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::RIGHT;
						_CurRoom->Type = ROOM_TYPE::RIGHT;
					}
					else if (m_Map[_mapPos.y][_mapPos.x] == (UINT)ROOM_TYPE::TOP_BOTTOM)
					{
						m_Map[_mapPos.y][_mapPos.x] = (UINT)ROOM_TYPE::TOP;
						_CurRoom->Type = ROOM_TYPE::TOP;
					}
					continue;
				}
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
				_nxtRoom->ParentPos = _mapPos;
				_nxtRoom->CurPos = _nxtPos;
				_nxtRoom->Spawn = m_RoomRef[(UINT)_type][0].Spawn;
				_nxtRoom->m_ParentDir = SPAWN_TYPE::TOP;
				_nxtRoom->Type = _type;
				_nxtRoom->_Parent = _CurRoom->_This;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.x), 900.f * (25.f - _nxtPos.y), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_CurRoom->_Child[(UINT)SPAWN_TYPE::BOTTOM] = _obj;
				m_MapInfo.push_back(_obj);
				m_MapRoomInfo.push_back(_nxtRoom);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				_nxtRoom->_This = _obj;
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::TOP:
				_nxtPos = _CurRoom->CurPos;
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
				_nxtRoom->ParentPos = _mapPos;
				_nxtRoom->CurPos = _nxtPos;
				_nxtRoom->Spawn = m_RoomRef[(UINT)_type][0].Spawn;
				_nxtRoom->Type = _type;
				_nxtRoom->m_ParentDir = SPAWN_TYPE::BOTTOM;
				_nxtRoom->_Parent = _CurRoom->_This;

				_obj = new GameObject;
				_tr = _obj->AddComponent<Transform>();
				_render = _obj->AddComponent<MeshRenderer>();

				_tr->SetRelativePosition(Vec4(1600.f * (25.f - _nxtPos.x), 900.f * (25.f - _nxtPos.y), 0.f, 1.f));
				_tr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 1.f));

				_render->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"dungeonBGMat"));

				_CurRoom->_Child[(UINT)SPAWN_TYPE::TOP] = _obj;
				m_MapRoomInfo.push_back(_nxtRoom);
				m_MapInfo.push_back(_obj);
				Object::Instantiate(_obj, (int)LAYER_TYPE::BACKGROUND);
				_nxtRoom->_This = _obj;
				q.push(_nxtRoom);
				break;
			case SPAWN_TYPE::END:
				break;
			default:
				break;
			}
		}
	}

	// 도어 생성
	for (size_t _idx = 0;_idx < m_MapRoomInfo.size();_idx++)
	{
		Room* _room = m_MapRoomInfo[_idx];
		Transform* _curTr = _room->_This->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _currentPos = _curTr->GetRelativePosition();

		// 내가 자식들을 향한 도어 생성
		for (UINT _dir = 0; _dir < (UINT)SPAWN_TYPE::END;_dir++)
		{
			if (_room->_Child[_dir] == nullptr)
				continue;

			GameObject* _door = nullptr;
			Vec4 _curPos = _currentPos;

			switch (SPAWN_TYPE(_dir))
			{
			case SPAWN_TYPE::RIGHT:
			{
				_door = new GameObject;
				Transform* _doortr = _door->AddComponent<Transform>();
				Collider2D* _doorcol = _door->AddComponent<Collider2D>();
				TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

				Transform* _childtr = _room->_Child[_dir]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

				_trigger->SetPlayerPos(_childtr->GetRelativePosition());
				_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

				float _width = 10.f;
				float _height = 30.f;

				_curPos.x -= 600.f + _width / 2.f;

				_doortr->SetRelativePosition(_curPos);
				_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
				Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
			}
			break;
			case SPAWN_TYPE::LEFT:
			{
				_door = new GameObject;
				Transform* _doortr = _door->AddComponent<Transform>();
				Collider2D* _doorcol = _door->AddComponent<Collider2D>();
				TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

				Transform* _childtr = _room->_Child[_dir]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

				_trigger->SetPlayerPos(_childtr->GetRelativePosition());
				_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

				float _width = 10.f;
				float _height = 30.f;

				_curPos.x += 600.f - _width / 2.f;
			
				_doortr->SetRelativePosition(_curPos);
				_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
				Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
			}
			break;
			case SPAWN_TYPE::BOTTOM:
			{
				_door = new GameObject;
				Transform* _doortr = _door->AddComponent<Transform>();
				Collider2D* _doorcol = _door->AddComponent<Collider2D>();
				TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

				Transform* _childtr = _room->_Child[_dir]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

				_trigger->SetPlayerPos(_childtr->GetRelativePosition());
				_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

				float _width = 30.f;
				float _height = 10.f;

				_curPos.y -= 300.f + _height / 2.f;

				_doortr->SetRelativePosition(_curPos);
				_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
				Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
			}
			break;
			case SPAWN_TYPE::TOP:
			{
				_door = new GameObject;
				Transform* _doortr = _door->AddComponent<Transform>();
				Collider2D* _doorcol = _door->AddComponent<Collider2D>();
				TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

				_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

				float _width = 30.f;
				float _height = 10.f;

				Transform* _childtr = _room->_Child[_dir]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

				_trigger->SetPlayerPos(_childtr->GetRelativePosition());

				_curPos.y += 300.f - _height / 2.f;

				_doortr->SetRelativePosition(_curPos);
				_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
				Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
			}
			break;
			case SPAWN_TYPE::END:
				break;
			default:
				break;
			}
		}

		// 내가 부모를 향한 도어 생성
		if (_room->_Parent == nullptr)
			continue;

		GameObject* _door = nullptr;
		Transform* _tr = _room->_This->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _curPos = _tr->GetRelativePosition();
		Transform* _parenttr = _room->_Parent->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		switch (_room->m_ParentDir)
		{
		case SPAWN_TYPE::RIGHT: 
		{
			_door = new GameObject;
			Transform* _doortr = _door->AddComponent<Transform>();
			Collider2D* _doorcol = _door->AddComponent<Collider2D>();
			TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

			_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

			float _width = 10.f;
			float _height = 30.f;

			_trigger->SetPlayerPos(_parenttr->GetRelativePosition());

			_curPos.x += 600.f - _width / 2.f;

			_doortr->SetRelativePosition(_curPos);
			_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
			Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
		}
			break;
		case SPAWN_TYPE::LEFT:
		{
			_door = new GameObject;
			Transform* _doortr = _door->AddComponent<Transform>();
			Collider2D* _doorcol = _door->AddComponent<Collider2D>();
			TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

			_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

			float _width = 10.f;
			float _height = 30.f;

			_trigger->SetPlayerPos(_parenttr->GetRelativePosition());

			_curPos.x -= 600.f + _width / 2.f;

			_doortr->SetRelativePosition(_curPos);
			_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
			Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
		}
			break;
		case SPAWN_TYPE::BOTTOM:
		{
			_door = new GameObject;
			Transform* _doortr = _door->AddComponent<Transform>();
			Collider2D* _doorcol = _door->AddComponent<Collider2D>();
			TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

			_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

			float _width = 30.f;
			float _height = 10.f;

			_trigger->SetPlayerPos(_parenttr->GetRelativePosition());

			_curPos.y -= 300.f - _height / 2.f;

			_doortr->SetRelativePosition(_curPos);
			_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
			Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
		}
			break;
		case SPAWN_TYPE::TOP:
		{
			_door = new GameObject;
			Transform* _doortr = _door->AddComponent<Transform>();
			Collider2D* _doorcol = _door->AddComponent<Collider2D>();
			TriggerScript* _trigger = _door->AddComponent<TriggerScript>();

			_trigger->SetTriggerType(TRIGGER_TYPE::MOVE_TRIGGER);

			float _width = 30.f;
			float _height = 10.f;

			_trigger->SetPlayerPos(_parenttr->GetRelativePosition());

			_curPos.y += 300.f - _height / 2.f;

			_doortr->SetRelativePosition(_curPos);
			_doortr->SetRelativeScale(Vec4(_width, _height, 0.f, 1.f));
			Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
		}
			break;
		case SPAWN_TYPE::END:
			break;
		default:
			break;
		}

	}

	// Player
	GameObject* _player = new GameObject();
	_player->SetName(L"Player");
	Transform* tr = _player->AddComponent<Transform>();
	tr->SetRelativeScale(Vec4(96.f, 164.f, 1.f, 1.f));
	tr->SetRelativePosition(Vec4(400.f, 0.f, -5.f, 1.f));
	tr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_playerRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"PlayerMaterial"));

	Collider2D* _playercol = _player->AddComponent<Collider2D>();

	Object::Instantiate(_player, (int)LAYER_TYPE::PLAYER);

	_player->AddComponent<PlayerScript>();

	Animator2D* _animator = _player->AddComponent<Animator2D>();
	Ptr<Sprite> _sprite = AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerIdleFront");
	_animator->CreateAnimation(L"IdleFront", _sprite, Vec2(0.f, 0.f), Vec2(0.f, 0.f), Vec2(24.f, 41.f), Vec2(24.f, 41.f), 10, 6.f);
	_animator->Play(L"IdleFront");

	// Light2D
	GameObject* _light = new GameObject;
	_light->SetName(L"Light");
	Transform* _lightTr = _light->AddComponent<Transform>();

	LIght2D* _light2D = _light->AddComponent<LIght2D>();
	_light2D->SetColor(Vec4(1.f, 1.f, 1.f, 1.f));
	_light2D->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));
	_light2D->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	_light2D->SetRadius(1000.f);
	_light2D->SetAngle(40.f);

	_light->AddComponent<Light2DScript>();

	Object::Instantiate(_light, (UINT)LAYER_TYPE::LIGHT2D);

	// PostProcess Test
	GameObject* _postprocess = new GameObject;
	Transform* _posttr = _postprocess->AddComponent<Transform>();
	MeshRenderer* _postMR = _postprocess->AddComponent<MeshRenderer>();

	_postMR->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	_postMR->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"DistortionFilterMat"));

	_posttr->SetRelativePosition(Vec4(0.f, -150.f, -5.f, 0.f));
	_posttr->SetRelativeScale(Vec4(400.f, 200.f, 0.f, 0.f));

	Object::Instantiate(_postprocess, (UINT)LAYER_TYPE::BACKGROUND);

	CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TRIGGER);
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

void DungeonScene::CreateRoom()
{

}

void DungeonScene::CreateDoor(Vec2 _pos, Vec2 _nxtpos)
{
	GameObject* _door = new GameObject;
	Collider2D* _col = _door->AddComponent<Collider2D>();
	Transform* _tr = _door->AddComponent<Transform>();

	_tr->SetRelativePosition(Vec4(_pos.x, _pos.y, 0.f, 0.f));
	_tr->SetRelativeScale(Vec4(50.f, 50.f, 1.f, 0.f));

	//Object::Instantiate(_door, (int)LAYER_TYPE::TRIGGER);
}
