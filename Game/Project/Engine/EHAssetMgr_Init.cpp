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
	| Bilbording Mesh
	**********************/
	vtx vPoint;
	UINT _idx = 0;
	Mesh* _OnePointMesh = new Mesh;
	_OnePointMesh->Create(&vPoint, 1, &_idx, 1);
	AddAsset(_OnePointMesh, L"PointMesh");

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

	_defaultShader->Default_Create(_path, _vsEntry, _psEntry);

	_defaultShader->SetCullType(CULL_TYPE::NONE);
	_defaultShader->SetDSType(DS_TYPE::LESS);
	_defaultShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_defaultShader, L"DefaultShader");

	/*********************
	|	Player Shader
	*********************/
	GraphicShader* _pPlayerShader = new GraphicShader;

	_path = L"\\shader\\playerVS.hlsl";
	wstring _path2 = L"\\shader\\playerPS.hlsl";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_pPlayerShader->Default_Create(_path, _path2, _vsEntry, _psEntry);

	_pPlayerShader->SetCullType(CULL_TYPE::NONE);
	_pPlayerShader->SetDSType(DS_TYPE::LESS);
	_pPlayerShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_pPlayerShader, L"PlayerShader");

	/*********************
	|	UI Shader
	*********************/

	GraphicShader* _pUIShader = new GraphicShader;

	_path = L"\\shader\\UIVS.hlsl";
	_path2 = L"\\shader\\UIPS.hlsl";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_pUIShader->Default_Create(_path, _path2, _vsEntry, _psEntry);

	_pUIShader->SetCullType(CULL_TYPE::NONE);
	_pUIShader->SetDSType(DS_TYPE::LESS);
	_pUIShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_pUIShader, L"UIShader");

	/********************
	|	Effect Shader
	********************/
	GraphicShader* _effectShader = new GraphicShader;

	_path = L"\\shader\\std2d.fx";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_effectShader->Default_Create(_path, _vsEntry, _psEntry);

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

	_GrayFilter->Default_Create(_path, _vsEntry, _psEntry);

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

	DistortionFilter->Default_Create(_path, _vsEntry, _psEntry);

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

	OutlineFilter->Default_Create(_path, _vsEntry, _psEntry);

	OutlineFilter->SetCullType(CULL_TYPE::BACK);
	OutlineFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	OutlineFilter->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(OutlineFilter, L"OutlineFilterShader");

	/******************
	| Mozaic Shader
	| Mesh		: RectMesh
	| RS_TYPE	: CULL_BACK
	| DS_TYPE	: NO_TEST_NO_WRITE
	| BS_TYPE	: Default
	| Domain	: DOMAIN_POSTPROCESS
	******************/
	GraphicShader* MozaicFilter = new GraphicShader;

	_path = L"\\shader\\postprocessVS.hlsl";
	_path2 = L"\\shader\\postprocessPS.hlsl";
	_vsEntry = "VS_MOZAIC";
	_psEntry = "PS_MOZAIC";

	MozaicFilter->Default_Create(_path, _path2, _vsEntry, _psEntry);

	MozaicFilter->SetCullType(CULL_TYPE::BACK);
	MozaicFilter->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	MozaicFilter->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(MozaicFilter, L"MozaicFilter");

	/********************
	|	Debug Shader
	********************/
	GraphicShader* _debugShader = new GraphicShader;

	_path = L"\\shader\\debug2d.fx";
	_vsEntry = "VS_DebugShader";
	_psEntry = "PS_DebugShader";

	_debugShader->Default_Create(_path, _vsEntry, _psEntry);
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

	tileShader->Default_Create(_path, _vsEntry, _psEntry);
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
	string _GsEntry = "GS_Particle";
	string _HuEntry = "";
	string _DmEntry = "";
	_psEntry = "PS_Particle";


	particleShader->Default_Create(_path, _vsEntry, _psEntry);
	particleShader->Custom_Create(_path, _GsEntry, _HuEntry, _DmEntry);
	particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	particleShader->SetCullType(CULL_TYPE::NONE);
	particleShader->SetDSType(DS_TYPE::LESS);
	particleShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);
	particleShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(particleShader, L"ParticleShader");
}

void AssetMgr::CreateDefaultMaterial()
{
	wstring _path = PATH;
	_path += L"\\resource\\materialdata\\Material.txt";
	std::wifstream _pFile(_path.c_str());

	if (_pFile.is_open())
	{
		bool _temp = false;
		wstring _line = {};
		wstring _wName = {};
		wstring _WShader = {};
		wstring _wSpritePath = {};
		while (std::getline(_pFile, _line))
		{
			for (int i = 0;i < 3;i++)
			{
				if (i == 0)
				{
					_wName = _line;
				}
				else if (i == 1)
				{
					std::getline(_pFile, _line);
					_WShader = _line;
				}
				else
				{
					std::getline(_pFile, _line);
					_wSpritePath = _line;

					Material* _mat = new Material;
					_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(_WShader));
					_mat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(_wSpritePath));
					AddAsset<Material>(_mat, _wName);
				}
			}
		}
	}

	Ptr<Material> _mat = FindAsset<Material>(L"DebugMaterial");
	_mat->SetMaterialParam(AMBIENT, Vec4(0.5f, 0.4f, 0.3f, 1.f));

	///***************************
	//| DistortionFilter Material
	//****************************/
	//Material* _distortionMat = new Material;
	//_distortionMat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"DistortionFilterShader"));
	//_distortionMat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(L"WaterSprite"));
	//_distortionMat->SetMaterialParam(INT_0, 1);
	//_distortionMat->SetTexParam(TEX_1, AssetMgr::GetInst()->FindAsset<Sprite>(L"Noise3"));
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