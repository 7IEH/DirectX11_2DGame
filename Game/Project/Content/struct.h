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
	Content::Vec2				ParentRoom;
	Content::Vec2				CurRoom;

	vector<SpawnPoint>	Spawn;
	ROOM_TYPE			Type;

	GameObject*			_This;
	GameObject*			_Parent;
};