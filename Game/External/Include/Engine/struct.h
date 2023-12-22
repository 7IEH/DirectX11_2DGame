#pragma once


struct vtx
{
	Vec3 _Postion;
	Vec4 _Color;
	Vec2 _UV;
};

// SRT
struct tTransform
{
	Vec4 _Scale;
	Vec4 _Position;
	Vec3 _Rotation;
};

// MVP
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

struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};

// �ֺ���(global illmination�� ǥ���ϱ� ���� ������ ������ ǥ��), ǥ���� ��ħ(���ݻ�), �ݻ���
struct material
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec4	Reflect;
};


// DX11 Light ����
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

