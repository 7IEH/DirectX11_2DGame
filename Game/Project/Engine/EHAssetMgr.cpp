#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

/************************
|	AssetMgr
|	1. Asset Initialize
|	1-1. Sprite
|	1-2. Mesh
|	1-3. Shader
|	1-4. Material
************************/

AssetMgr::AssetMgr()
{

}

AssetMgr::~AssetMgr()
{
}

void AssetMgr::Awake()
{
	wstring _path = L"\\resource\\Example.png";
	Ptr<Sprite> _BackGroundSprite = Load<Sprite>(_path, L"BackGroundSprite");

	_path = L"\\resource\\NPC_Commander0.png";
	Ptr<Sprite> _PlayerSprite = Load<Sprite>(_path, L"PlayerSprite");

	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
}


void AssetMgr::CreateDefaultMesh()
{
	vtx triangle[4] = {};
	UINT idx[6] = {};

	/**********************
	| Default Rect Mesh
	**********************/
	Mesh* _Rectmesh = new Mesh;

	// Triangle Test------------------------------
	triangle[0]._Postion = Vec3(-0.5f, 0.5f, 0.f);
	triangle[0]._Color = Vec4(1.f, 0.f, 0.f, 1.f);
	triangle[0]._UV = Vec2(0.f, 0.f);

	triangle[1]._Postion = Vec3(0.5f, 0.5f, 0.f);
	triangle[1]._Color = Vec4(0.f, 1.f, 0.f, 1.f);
	triangle[1]._UV = Vec2(1.f, 0.f);

	triangle[2]._Postion = Vec3(0.5f, -0.5f, 0.f);
	triangle[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[2]._UV = Vec2(1.f, 1.f);

	triangle[3]._Postion = Vec3(-0.5f, -0.5f, 0.f);
	triangle[3]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[3]._UV = Vec2(0.f, 1.f);
	// --------------------------------------------

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;

	idx[3] = 0;
	idx[4] = 2;
	idx[5] = 3;

	_Rectmesh->Create(triangle, 4, idx, 6);

	AddAsset(_Rectmesh, L"DefaultRectMesh");


	/***************************
	| Default Debug Rect Mesh
	***************************/
	Mesh* _DebugRectMesh = new Mesh;

	// Triangle Test------------------------------
	triangle[0]._Postion = Vec3(-0.5f, 0.5f, 0.f);
	triangle[0]._Color = Vec4(1.f, 0.f, 0.f, 1.f);
	triangle[0]._UV = Vec2(0.f, 0.f);

	triangle[1]._Postion = Vec3(0.5f, 0.5f, 0.f);
	triangle[1]._Color = Vec4(0.f, 1.f, 0.f, 1.f);
	triangle[1]._UV = Vec2(1.f, 0.f);

	triangle[2]._Postion = Vec3(0.5f, -0.5f, 0.f);
	triangle[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[2]._UV = Vec2(1.f, 1.f);

	triangle[3]._Postion = Vec3(-0.5f, -0.5f, 0.f);
	triangle[3]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[3]._UV = Vec2(0.f, 1.f);
	// --------------------------------------------

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 3;
	idx[4] = 0;

	_DebugRectMesh->Create(triangle, 4, idx, 5);

	AddAsset(_DebugRectMesh, L"DefaultRectMesh_Debug");
}

void AssetMgr::CreateDefaultShader()
{
	/******************
	| Default Shader
	******************/
	GraphicShader* _defaultShader = new GraphicShader;

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	_defaultShader->Create(_path, _vsEntry, _psEntry);
	_defaultShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_defaultShader, L"DefaultShader");

	/******************
	| Debug Shader
	******************/
	GraphicShader* _debugShader = new GraphicShader;

	_path = L"\\shader\\debug2d.fx";
	_vsEntry = "VS_DebugShader";
	_psEntry = "PS_DebugShader";

	_debugShader->Create(_path, _vsEntry, _psEntry);
	_debugShader->SetBlendType(BLEND_TYPE::DEFAULT);
	_debugShader->SetCullType(CULL_TYPE::NONE);
	_debugShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	_debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddAsset(_debugShader, L"DebugShader");
}

void AssetMgr::CreateDefaultMaterial()
{
	/******************
	| BackGround Material
	******************/
	Material* _backgroundMaterial = new Material;
	_backgroundMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_backgroundMaterial->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_backgroundMaterial->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_backgroundMaterial->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_backgroundMaterial->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_backgroundMaterial->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"BackGroundSprite"));

	/******************
	| Player Material
	******************/
	Material* _playerMaterial = new Material;
	_playerMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_playerMaterial->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_playerMaterial->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_playerMaterial->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_playerMaterial->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerSprite"));

	/******************
	| Debug Material
	******************/
	Material* _debugMaterial = new Material;
	_debugMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DebugShader"));
	_debugMaterial->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));

	AddAsset(_backgroundMaterial, L"BackGroundMaterial");
	AddAsset(_playerMaterial, L"PlayerMaterial");
	AddAsset(_debugMaterial, L"DebugMaterial");
}