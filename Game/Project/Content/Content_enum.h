#pragma once

/***************************
|	Content Struct Type
***************************/

/****************
|	Room Type
****************/
enum class ROOM_TYPE
{
	NO_ROOM,
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	LEFT_RIGHT,
	TOP_BOTTOM,
	ENTRY_ROOM,
	END
};

/****************
|	Spawn Type
****************/
enum class SPAWN_TYPE
{
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	END
};

/*******************
|	Trigger Type
*******************/
enum class TRIGGER_TYPE
{
	MOVE_TRIGGER,
	CHEST_TRIGGER,
	DUNGEONENTRANCE,
	TOWN,
	WILLROOM,
	GOLEM_DUNGEON,
	GOLEM_BOSS_MOVE_TRIGGER,
	END,
};

/*******************
|	Dungeon Type
*******************/
enum class DungeonType
{
	GOLEM,
	None,
};