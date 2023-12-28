#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

AssetMgr::AssetMgr()
{

}

AssetMgr::~AssetMgr()
{
}

void AssetMgr::Awake()
{
	#pragma region Basic Array
	vtx triangle[4] = {};
	UINT idx[6] = {};
	#pragma endregion

	#pragma region MeshSetting
	Mesh* _mesh = new Mesh;
	Mesh* _texture_mesh = new Mesh;

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

	_mesh->Create(triangle, 4, idx, 6);

	vtx triangle2[4] = {};
	UINT idx2[6] = {};
	// Triangle Test------------------------------
	triangle2[0]._Postion = Vec3(-0.5f, 0.5f, 0.f);
	triangle2[0]._Color = Vec4(1.f, 0.f, 0.f, 1.f);
	triangle2[0]._UV = Vec2(0.f, 0.f);

	triangle2[1]._Postion = Vec3(0.5f, 0.5f, 0.f);
	triangle2[1]._Color = Vec4(0.f, 1.f, 0.f, 1.f);
	triangle2[1]._UV = Vec2(1.f, 0.f);

	triangle2[2]._Postion = Vec3(0.5f, -0.5f, 0.f);
	triangle2[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle2[2]._UV = Vec2(1.f, 1.f);

	triangle2[3]._Postion = Vec3(-0.5f, -0.5f, 0.f);
	triangle2[3]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle2[3]._UV = Vec2(0.f, 1.f);
	// --------------------------------------------

	idx2[0] = 0;
	idx2[1] = 1;
	idx2[2] = 2;

	idx2[3] = 0;
	idx2[4] = 2;
	idx2[5] = 3;

	_texture_mesh->Create(triangle2, 4, idx2, 6);

	AddAsset(_mesh, L"BackGroundMesh");
	AddAsset(_texture_mesh, L"PlayerMesh");
	#pragma endregion

	#pragma region ShaderSetting
	GraphicShader* _shader = new GraphicShader;
	GraphicShader* _texture_shader = new GraphicShader;

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	_shader->Create(_path, _vsEntry, _psEntry);
	_shader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	_path = L"\\shader\\std2d.fx";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_texture_shader->Create(_path, _vsEntry, _psEntry);
	_texture_shader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_shader, L"BackGroundShader");
	AddAsset(_texture_shader, L"PlayerShader");
	#pragma endregion
	
	#pragma region SpriteSetting
	_path = L"\\resource\\Example.png";
	Ptr<Sprite> _BackGroundSprite = Load<Sprite>(_path,L"BackGroundSprite");

	_path = L"\\resource\\NPC_Commander0.png";
	Ptr<Sprite> _PlayerSprite = Load<Sprite>(_path, L"PlayerSprite");
	#pragma endregion

	#pragma region Mateiral
	Material* _backgroundMaterial = new Material;
	_backgroundMaterial->SetGraphicShader(_shader);
	_backgroundMaterial->SetTexParam(TEX_0, _BackGroundSprite);

	Material* _playerMaterial = new Material;
	_playerMaterial->SetGraphicShader(_shader);
	_playerMaterial->SetTexParam(TEX_0, _PlayerSprite);

	AddAsset(_backgroundMaterial, L"BackGroundMaterial");
	AddAsset(_playerMaterial, L"PlayerMaterial");
	#pragma endregion


	#pragma region RandMapTest
	Mesh* _test_mesh = new Mesh;

	vtx triangle3[4] = {};
	UINT idx3[6] = {};
	// Triangle Test------------------------------
	triangle3[0]._Postion = Vec3(-0.5f, 0.5f, 0.f);
	triangle3[0]._Color = Vec4(1.f, 0.f, 0.f, 1.f);
	triangle3[0]._UV = Vec2(0.f, 0.f);

	triangle3[1]._Postion = Vec3(0.5f, 0.5f, 0.f);
	triangle3[1]._Color = Vec4(0.f, 1.f, 0.f, 1.f);
	triangle3[1]._UV = Vec2(1.f, 0.f);

	triangle3[2]._Postion = Vec3(0.5f, -0.5f, 0.f);
	triangle3[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle3[2]._UV = Vec2(1.f, 1.f);

	triangle3[3]._Postion = Vec3(-0.5f, -0.5f, 0.f);
	triangle3[3]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle3[3]._UV = Vec2(0.f, 1.f);
	// --------------------------------------------

	idx3[0] = 0;
	idx3[1] = 1;
	idx3[2] = 2;

	idx3[3] = 0;
	idx3[4] = 2;
	idx3[5] = 3;

	_test_mesh->Create(triangle3, 4, idx3, 6);

	AddAsset(_test_mesh, L"randMesh");

	// RandomMap Test
	GraphicShader* _ranshader = new GraphicShader;

	_path = L"\\shader\\std2d.fx";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	//_ranshader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	_ranshader->Create(_path, _vsEntry, _psEntry);
	AddAsset(_ranshader, L"randShader");

	_path = L"\\resource\\ranTest.png";
	Ptr<Sprite> _ranSprite = Load<Sprite>(_path, L"ranSprite");

	Material* _randMaterial = new Material;
	_randMaterial->SetTexParam(TEX_0,_ranSprite);
	_randMaterial->SetGraphicShader(_ranshader);
	AddAsset(_randMaterial, L"randMat");
	#pragma endregion
}
