#pragma once


struct vtx
{
	Vec3 _Postion;
	Vec4 _Color;
	Vec2 _UV;
};

struct tTransform
{
	Vec4 _Position;
	Vec4 _Scale;
};

struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};