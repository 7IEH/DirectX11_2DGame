#pragma once

/******************
Struct Type
******************/

// Task Sturct
struct EHTask
{
	TASK_TYPE _Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
};

// Vertex Struct
struct vtx
{
	Vec3 _Postion;
	Vec4 _Color;
	Vec2 _UV;
};

// SRT Struct
struct tTransform
{
	Vec4 _Scale;
	Vec4 _Position;
	Vec3 _Rotation;
};

// MVP Struct
struct transform
{
	Matrix	World;
	Matrix	WorldInv;

	Matrix	View;
	Matrix	ViewInv;

	Matrix	Projection;
	Matrix	ProjInv;

	Matrix	WV;
	Matrix	WVP;
};

// Key Struct
struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};

struct LightMateiral
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec4	Reflect;
};

// 주변광(global illmination을 표현하기 위해 일정한 값으로 표현), 표면의 거침(난반사), 반사율
struct material
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	LightMateiral vLightMat;
	Matrix matArr[4];

	int	Sprite[(UINT)TEX_PARAM::END];
	int padding[2];
};

struct NomralVector
{
	Vec3 Nomral;
	float pad;
};

// DX11 Light 예제
struct DirectinalLight
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec3	Direction;
	float	pad;
};

struct PointLight
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec3	Position;
	float	Range;
	Vec3	Att;
	float	pad;
};

struct SpotLight
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec3	Position;
	float	Range;
	Vec3	Direction;
	float	Spot;
	Vec3	Att;
	float	pad;
};

struct tLight
{
	DirectinalLight _DL;
	PointLight _PL;
	SpotLight _SL;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE	eShape;

	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fDuration;
	float		fLifeTime;
	bool		bDepthTest;
};

struct tAnimationInfo
{
	Vec2 _OffsetSize;
	Vec2 _LeftTop;
	Vec2 _SliceSize;
	Vec2 _BackGround;
	int	 _AnimUse;
	Vec3  _Padding;
};