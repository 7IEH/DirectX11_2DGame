#pragma once
class RoomManager
	:public Singleton<RoomManager>
{
	Single(RoomManager)
private:
	vector<Room>		m_RoomRef[(UINT)ROOM_TYPE::END];

public:
	vector<Room> GetRoomRef(ROOM_TYPE _type) { return m_RoomRef[(UINT)_type]; }

public:
	void Awake();
};

