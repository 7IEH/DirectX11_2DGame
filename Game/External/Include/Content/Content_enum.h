#pragma once

/***************************
|	Content Struct Type
***************************/

// Room Type
enum class ROOM_TYPE
{
	padding,
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

// Spawn Type
enum class SPAWN_TYPE
{
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	END
};

// Trigger Type
enum class TRIGGER_TYPE
{
	MOVE_TRIGGER,
	END,
};