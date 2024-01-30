#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

#include "EHDebugMgr.h"
#include "EHPathMgr.h"

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
	CreateSprite();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
}

void AssetMgr::CreateSprite()
{
	wstring _path = PATH;
	_path += L"\\resource\\spritedata\\SpriteData.txt";
	std::ifstream _file(string(_path.begin(), _path.end()).data());

	wstring _relativePath = L"";
	wstring _name = L"";

	if (_file.is_open())
	{
		bool _temp = false;
		string _line;
		while (std::getline(_file, _line))
		{
			if (!_temp)
			{
				_name = wstring(_line.begin(), _line.end());
				_temp = true;
			}
			else
			{
				_relativePath = wstring(_line.begin(), _line.end());

				if (_relativePath == L"")
					return;
				Ptr<Sprite> _BackGroundSprite = Load<Sprite>(_relativePath, _name);
				_temp = false;
			}
		}
		_file.close();
	}
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
	| RS_TYPE	: CULL_BACK
	| DS_TYPE	: NO_TEST_NO_WRITE
	| BS_TYPE	: Default
	| Domain	: DOMAIN_POSTPROCESS
	******************/
	GraphicShader* _GrayFilter = new GraphicShader;

	_path = L"\\shader\\postprocess.fx";
	_vsEntry = "VS_GrayFilter";
	_psEntry = "PS_GrayFilter";

	_GrayFilter->Create(_path, _vsEntry, _psEntry);

	_GrayFilter->SetCullType(CULL_TYPE::BACK);
	_GrayFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	AddAsset(_GrayFilter, L"GrayFilterShader");

	/******************
	| Distortion Shader
	| Mesh		: RectMesh
	| RS_TYPE	: CULL_BACK
	| DS_TYPE	: NO_TEST_NO_WRITE
	| BS_TYPE	: Default
	| Domain	: DOMAIN_POSTPROCESS
	******************/
	GraphicShader* DistortionFilter = new GraphicShader;

	_path = L"\\shader\\postprocess.fx";
	_vsEntry = "VS_DistortionFilter";
	_psEntry = "PS_DistortionFilter";

	DistortionFilter->Create(_path, _vsEntry, _psEntry);

	DistortionFilter->SetCullType(CULL_TYPE::BACK);
	DistortionFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	DistortionFilter->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(DistortionFilter, L"DistortionFilterShader");

	/******************
	| Outline Shader
	| Mesh		: RectMesh
	| RS_TYPE	: CULL_BACK
	| DS_TYPE	: NO_TEST_NO_WRITE
	| BS_TYPE	: Default
	| Domain	: DOMAIN_POSTPROCESS
	******************/
	GraphicShader* OutlineFilter = new GraphicShader;

	_path = L"\\shader\\postprocess.fx";
	_vsEntry = "VS_OutLineFilter";
	_psEntry = "PS_OutLineFilter";

	OutlineFilter->Create(_path, _vsEntry, _psEntry);

	OutlineFilter->SetCullType(CULL_TYPE::BACK);
	OutlineFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	OutlineFilter->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(OutlineFilter, L"OutlineFilterShader");

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

	// =============
	// TileMapShader
	// =============
	GraphicShader* tileShader = new GraphicShader;
	_path = L"\\shader\\tilemap.fx";
	_vsEntry = "VS_TileMap";
	_psEntry = "PS_TileMap";

	tileShader->Create(_path, _vsEntry, _psEntry);
	tileShader->SetCullType(CULL_TYPE::NONE);
	tileShader->SetDSType(DS_TYPE::LESS);
	tileShader->SetBlendType(BLEND_TYPE::DEFAULT);

	AddAsset(tileShader, L"TileMapShader");

	/*************************
	|	ParticleShader
	*************************/
	GraphicShader* particleShader = new GraphicShader;
	_path = L"\\shader\\particle.fx";
	_vsEntry = "VS_Particle";
	_psEntry = "PS_Particle";

	particleShader->Create(_path, _vsEntry, _psEntry);
	particleShader->SetCullType(CULL_TYPE::NONE);
	particleShader->SetDSType(DS_TYPE::N0_WRITE);
	particleShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);
	particleShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(particleShader, L"ParticleShader");
}

void AssetMgr::CreateDefaultMaterial()
{
	/******************
	| BackGround Material
	******************/
	Material* _backgroundMaterial = new Material;
	_backgroundMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	//_backgroundMaterial->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"BackGroundSprite"));

	/******************
	| Player Material
	******************/
	Material* _playerMaterial = new Material;
	_playerMaterial->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_playerMaterial->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerTestSprite"));
	_playerMaterial->SetMaterialParam(INT_0, 1);

	/****************************
	| Player OutLine Material
	****************************/
	Material* _OutLineMat = new Material;
	_OutLineMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"OutlineFilterShader"));

	/******************
	| Right Door Material
	******************/
	Material* _rightDoorMat = new Material;
	_rightDoorMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_rightDoorMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartRightDoor"));

	/******************
	| Left Door Material
	******************/
	Material* _leftDoorMat = new Material;
	_leftDoorMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_leftDoorMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartLeftDoor"));

	/******************
	| Center Door Material
	******************/
	Material* centerLineMat = new Material;
	centerLineMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	centerLineMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"StartLine"));

	/******************
	| Logo Door Material
	******************/
	Material* LogoMat = new Material;
	LogoMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
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

	/******************
	| WaterGround
	******************/
	Material* _waterGround = new Material;
	_waterGround->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DefaultShader"));
	_waterGround->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"WaterSprite"));

	/***************************
	| PostProcess TEST Material
	***************************/
	Material* _postProcess = new Material;
	_postProcess->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"GrayFilterShader"));

	/***************************
	| DistortionFilter Material
	****************************/
	Material* _distortionMat = new Material;
	_distortionMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DistortionFilterShader"));
	_distortionMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"WaterSprite"));
	_distortionMat->SetMaterialParam(INT_0, 1);
	_distortionMat->SetTexParam(TEX_1, AssetMgr::GetInst()->FindAsset<Sprite>(L"Noise3"));

	/***************************
	| TileMap Material
	***************************/
	Material* _tileMap = new Material;
	_tileMap->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"TileMapShader"));
	AddAsset(_tileMap, L"TileMapMat");

	/***************************
	| Particle Material
	***************************/
	Material* _particleMat = new Material;
	_particleMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"ParticleShader"));
	AddAsset(_particleMat, L"ParticleMat");


	AddAsset(_postProcess, L"GrayFilterMat");
	AddAsset(_distortionMat, L"DistortionFilterMat");
	AddAsset(_waterGround, L"WaterGroundMat");
	AddAsset(_OutLineMat, L"OutLineMat");

	AddAsset(_backgroundMaterial, L"BackGroundMaterial");
	AddAsset(_playerMaterial, L"PlayerMaterial");
	AddAsset(_debugMaterial, L"DebugMaterial");

	AddAsset(_rightDoorMat, L"StartRightDoorMat");
	AddAsset(_leftDoorMat, L"StartLeftDoorMat");
	AddAsset(centerLineMat, L"StartLineMat");
	AddAsset(LogoMat, L"GameLogoMat");

	AddAsset(dungeonBG, L"dungeonBGMat");
}

#include "EHTestComputeShader.h"
#include "EHParticleUpdate.h"
void AssetMgr::CreateDefaultComputeShader()
{
	Ptr<ComputeShader> pShader = nullptr;

	// SetColorShader
	pShader = new TestComputeShader;
	AddAsset(pShader.Get(), L"SetColorShader");

	pShader = new ParticleUpdate;
	AddAsset(pShader.Get(), L"ParticleUpdateShader");
}