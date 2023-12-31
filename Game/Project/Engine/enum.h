/******************
enum class Type
******************/

// Task Type
enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : LEVEL_TYPE
	LEVEL_CHANGE,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,
};

// Key Type
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

// Key State Type
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
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	MATERIAL,
	TEXTURE,
	SPRITE,
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
	NORMANL,
	ANIMATION,
	END,
};

// Layer Type
enum class LAYER_TYPE
{
	CAMERA,
	LIGHT2D,
	BACKGROUND,
	PLAYER,
	MONSTER,
	END,
	NON_SELECT
};

// Projection Type
enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
	END
};

// Direction Type
enum class DIRECTION_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};

// Culling Type
enum class CULL_TYPE
{
	BACK,
	FRONT,
	NONE,
	WIRE,
	END
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,					// 작은게 통과			깊이 기록 0
	LESS_EQUAL,				// 작거나 같으면 통과	깊이 기록 0

	GREATER,				// 큰게 통과			깊이 기록 0
	GREATER_EQUAL,			// 크거나 같으면 통과	깊이 기록 0

	NO_TEST,				// 깊이 테스트X			깊이 기록 0
	N0_WRITE,				// 깊이 테스트O			깊이 기록 X

	NO_TEST_NO_WRITE,		// 깊이 테스트X			깊이 기록X
	END
};

// BlendState Type
enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLENDING,
	ONE_ONE,
	END
};

// SamplerState Type
enum class  SAMPLER_TYPE
{
	Default,
	POINT,
	END,
};

// Camera Type
enum class CAMERA_TYPE
{
	MAIN_CAMERA,
	DEBUG_CAMERA,
	UI_CAMERA,
	WORLD_CAMERA,
	END,
	NON_TYPE
};

// Scalar Param
enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	AMBIENT,
	DIFFUSE,
	SPECULAR,
	REFLECT,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

// Texture Param
enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

// Debug Shape
enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,

	CUBE,
	SPHERE,
};

// Map Generator Room Direction
enum class DIR_ROOM 
{
	RIGHT,	
	LEFT,
	TOP,
	BOTTOM,
	CLOSED,
	END
};

// IMGUI FONT TYPE
enum class FONT_TYPE
{
	Default,
	Maple,
	END,
};

// COLLIDER TYPE
enum class COLLIDER_TYPE
{
	BOXCOLLIDER2D,
	CIRCLECOLLDIER2D,
	BOXCOLLIDER,
	SPHERECOLLDIER,
	END,
};

// STRUCTURED BUFFER TYPE
enum class STRUCTURED_TYPE
{
	READ_ONLY,
	READ_WRITE,
	END
};