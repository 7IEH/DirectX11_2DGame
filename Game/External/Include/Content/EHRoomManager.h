#pragma once

class GameObject;
class RoomManager
	:public Singleton<RoomManager>
{
	Single(RoomManager)
private:
	vector<Room>		m_RoomRef[(UINT)ROOM_TYPE::END];

public:
	vector<Room> GetRoomRef(ROOM_TYPE _type) { return m_RoomRef[(UINT)_type]; }

public:
	void	AllRoomSetting(vector<Room>& _mapInfo);

private:
	void	CreateRightRoom(GameObject* _backGround);
	void	CreateLeftRoom(GameObject* _backGround);
	void	CreateTopRoom(GameObject* _backGround);
	void	CreateBottomRoom(GameObject* _backGround);

	void	CreateLeftTopRoom(GameObject* _backGround);
	void	CreateRightTopRoom(GameObject* _backGround);
	void	CreateLeftBottomRoom(GameObject* _backGround);
	void	CreateRightBottomRoom(GameObject* _backGround);

	void	CreateLeftRightRoom(GameObject* _backGround);
	void	CreateTopBottomRoom(GameObject* _backGround);

public:
	void Awake();
};

