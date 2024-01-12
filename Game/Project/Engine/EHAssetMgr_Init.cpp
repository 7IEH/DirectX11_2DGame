#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

#include "EHDebugMgr.h"

/************************
|	AssetMgr
|	1. Asset Initialize
|	1-1. Sprite(D2D11 Texture)
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

	_path = L"\\resource\\NPC_CommanderSheet.png";
	Ptr<Sprite> _PlayerIdleSprite = Load<Sprite>(_path, L"PlayerIdleSprite");

	_path = L"\\resource\\Title\\MainBG\\MenuAtlas1.png";
	Ptr<Sprite> _BackGroundAtlas1 = Load<Sprite>(_path, L"_BackGroundAtlas1");

	_path = L"\\resource\\Title\\MainBG\\MenuAtlas2.png";
	Ptr<Sprite> _BackGroundAtlas2 = Load<Sprite>(_path, L"_BackGroundAtlas2");

	_path = L"\\resource\\Title\\MainBG\\GUI_StartScreen_LeftDoor.png";
	Ptr<Sprite> LeftDoor = Load<Sprite>(_path, L"StartLeftDoor");

	_path = L"\\resource\\Title\\MainBG\\GUI_StartScreen_RightDoor.png";
	Ptr<Sprite> RightDoor = Load<Sprite>(_path, L"StartRightDoor");

	_path = L"\\resource\\Title\\MainBG\\GUI_StartScreen_Line.png";
	Ptr<Sprite> StartLine = Load<Sprite>(_path, L"StartLine");

	_path = L"\\resource\\Title\\MainBG\\GameLogo.png";
	Ptr<Sprite> GameLogo = Load<Sprite>(_path, L"GameLogo");

	_path = L"\\resource\\photoshopver.png";
	Ptr<Sprite> DungeonBackGround = Load<Sprite>(_path, L"DungeonBG");

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

	/***************************
	| Default Debug Circle Mesh
	***************************/
	Mesh* _DebugCircleMesh = new Mesh;

	vector<vtx> vecVtx;
	vector<UINT> vecIdx;
	vtx v;

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (UINT i = 0;i <= iSlice;i++)
	{
		fTheta = (XM_2PI / iSlice) * i;
		v._Postion = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v._Color = Vec4(1.f, 1.f, 1.f, 1.f);
		v._UV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (int i = 0;i < vecVtx.size();i++)
	{
		vecIdx.push_back(i);
	}

	_DebugCircleMesh->Create(vecVtx.data(), (UINT)vecIdx.size(), vecIdx.data(), (UINT)vecIdx.size());

	AddAsset(_DebugCircleMesh, L"DefaultCircleMesh_Debug");
}

void AssetMgr::CreateDefaultShader()
{
	/*********************
	|	Default Shader
	*********************/
	GraphicShader* _defaultShader = new GraphicShader;

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	_defaultShader->Create(_path, _vsEntry, _psEntry);

	_defaultShader->SetCullType(CULL_TYPE::NONE);
	_defaultShader->SetDSType(DS_TYPE::LESS);
	_defaultShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_defaultShader, L"DefaultShader");

	/********************
	|	Effect Shader
	********************/
	GraphicShader* _effectShader = new GraphicShader;

	_path = L"\\shader\\std2d.fx";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_effectShader->Create(_path, _vsEntry, _psEntry);

	_effectShader->SetCullType(CULL_TYPE::NONE);
	_effectShader->SetDSType(DS_TYPE::LESS);
	_effectShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_effectShader, L"EffectShader");

	/******************
	| GrayFilter Shader
	| Mesh		: RectMesh
	| RS_TYPE	: CALL_BACK
	| DS_TYPE	: NO_TEST_NO_WRITE
	| BS_TYPE	: Default
	| Domain	: DOMAIN_POSTPROCESS
	******************/
	GraphicShader* _testFilter = new GraphicShader;

	_path = L"\\shader\\postprocess.fx";
	_vsEntry = "VS_GrayFilter";
	_psEntry = "PS_GrayFilter";

	_testFilter->Create(_path, _vsEntry, _psEntry);

	_testFilter->SetCullType(CULL_TYPE::BACK);
	_testFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	AddAsset(_testFilter, L"GrayFilterShader");

	/********************
	|	Debug Shader
	********************/
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
	| Right Door Material
	******************/
	Material* _rightDoorMat = new Material;
	_rightDoorMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_rightDoorMat->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_rightDoorMat->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_rightDoorMat->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_rightDoorMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartRightDoor"));

	/******************
	| Left Door Material
	******************/
	Material* _leftDoorMat = new Material;
	_leftDoorMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_leftDoorMat->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_leftDoorMat->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_leftDoorMat->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	_leftDoorMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartLeftDoor"));

	/******************
	| Center Door Material
	******************/
	Material* centerLineMat = new Material;
	centerLineMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	centerLineMat->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	centerLineMat->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	centerLineMat->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	centerLineMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartLine"));

	/******************
	| Logo Door Material
	******************/
	Material* LogoMat = new Material;
	LogoMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	LogoMat->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	LogoMat->SetMaterialParam(DIFFUSE, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	LogoMat->SetMaterialParam(SPECULAR, Vec4(0.5f, 0.4f, 0.3f, 1.f));
	LogoMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"GameLogo"));

	Material* dungeonBG = new Material;
	dungeonBG->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	dungeonBG->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"DungeonBG"));

	/******************
	| Debug Material
	******************/
	Material* _debugMaterial = new Material;
	_debugMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DebugShader"));
	_debugMaterial->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));

	/***************************
	| PostProcess TEST Material
	***************************/
	Material* _postProcess = new Material;
	_postProcess->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"GrayFilterShader"));

	AddAsset(_postProcess, L"GrayFilterMat");

	AddAsset(_backgroundMaterial, L"BackGroundMaterial");
	AddAsset(_playerMaterial, L"PlayerMaterial");
	AddAsset(_debugMaterial, L"DebugMaterial");

	AddAsset(_rightDoorMat, L"StartRightDoorMat");
	AddAsset(_leftDoorMat, L"StartLeftDoorMat");
	AddAsset(centerLineMat, L"StartLineMat");
	AddAsset(LogoMat, L"GameLogoMat");

	AddAsset(dungeonBG, L"dungeonBGMat");
}