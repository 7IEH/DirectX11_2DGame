#pragma once

#include "Content_enum.h"
#include "content_struct.h"

class GameObject;
class RoomManager
	:public Singleton<RoomManager>
{
	Single(RoomManager)
private:
	vector<Room>			m_vRoomRef[(UINT)ROOM_TYPE::END];
	int						m_iMap[50][50];

	wstring					m_wDoorOpenAnim;
	wstring					m_wDoorCloseAnim;

	map<wstring, Room>			m_mMapRoomInfo;

	GameObject* m_pPlayerObj;

	Content::Vec2					m_vCurPos;
	bool							m_bBossClose;

public:
	vector<Room> GetGolemRoomRef(ROOM_TYPE _type) { return m_vRoomRef[(UINT)_type]; }
	void		 SetCurPos(Content::Vec2 _vCurPos) { m_vCurPos = _vCurPos; }
	Content::Vec2		 GetCurPos() { return m_vCurPos; }

	void		SetBossClose(bool _bBossClose) { m_bBossClose = m_bBossClose; }

private:
	void	CreateRoomRef();
	void	GolemDungeonStart();
	void	GolemDungeonUpdate();
	void	CreateBossRoom(const wstring& _wOpenAnim, const wstring& _wCloseAnim);

	void	LoadPrefab();

private:
	void    CreateDoorIndividual(GameObject* _pDoor, const Vec4& _vPos, const Vec3& _vRot, const wstring& _wOpenAnim, const wstring& _wCloseAnim, bool _bEntryDoor, bool _bBossDoor, TRIGGER_TYPE _eType = TRIGGER_TYPE::MOVE_TRIGGER);
	void	CreateRoomIndividualRef(const vector<SPAWN_TYPE>& _vSpawnType, const ROOM_TYPE _eType);
	void	CreateRoomObject(SPAWN_TYPE _eType, OUT Room& _sRoom, OUT Room& _sNxtRoom, OUT bool& _bFlag);
	void	CreateDoor(const wstring& _wOpenAnim, const wstring& _wCloseAnim);

public:
	void Awake();
	void Start(DungeonType _eType);
	void Update(DungeonType _eType);
};

