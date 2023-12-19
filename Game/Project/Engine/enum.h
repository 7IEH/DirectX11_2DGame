enum KEY
{
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,

	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	TAB,
	LSHIFT,
	LALT,
	LCTRL,
	SPACE,

	ESC,
	ENTER,
	BACK,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	LBTN,
	RBTN,
	MBTN,

	KEY_END,
};

enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

// Component Type
enum class COMPONENT_TYPE
{
	TRANSFORM,
	LIGHT2D,
	COLLIDER2D,
	ANIMATOR2D,
	RENDERER,
	CAMERA,
	END,

	SCRIPT
};

// Renderer Type
enum class RENDERER_TYPE
{
	MESHRENDERER,
	END
};

// Buffer Type
enum class BUFFER_TYPE
{
	VERTEX,
	INDEX,
	CONSTANT,
	END
};

// Asset Type
enum class ASSET_TYPE 
{
	MESH,
	SHADER,
	MATERIAL,
	SOUND,
	END,
};

// Shader Type
enum class SHADER_TYPE
{
	VERTEX,
	PIXEL,
	HULL,
	DOMAlN,
	GEOMETRY,
	END,
};

// Constant Buffer Type
enum class CONSTANT_TYPE
{
	TRANSFORM,
	MATERIAL,
	LIGHT,
	ANIMATION,
	END,
};

enum class LAYER_TYPE
{
	CAMERA,
	LIGHT2D,
	BACKGROUND,
	PLAYER,
	END
};

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
	END
};

enum class DIRECTION_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};