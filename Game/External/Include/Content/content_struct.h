#pragma once

/*******************
| Content Struct
*******************/

namespace Content
{
	struct Vec2
	{
		int x;
		int y;

		Vec2()
			:x(0)
			, y(0)
		{

		}

		Vec2(int _x, int _y)
			:x(_x)
			, y(_y)
		{

		}
	};
}

struct SpawnPoint
{
	SPAWN_TYPE _SpawnPos;
};

struct Room
{
	Content::Vec2				ParentPos;
	Content::Vec2				CurPos;

	vector<SpawnPoint>			Spawn;
	ROOM_TYPE					Type;

	SPAWN_TYPE					m_ParentDir;

	GameObject*					_This;
	GameObject*					_Parent;
	GameObject*					_Child[(UINT)SPAWN_TYPE::END];

	vector<GameObject*>			_pMonster;
	vector<GameObject*>			_pDoor;

	bool						_bMapVisible;
};