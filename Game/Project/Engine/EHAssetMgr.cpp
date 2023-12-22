#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

AssetMgr::AssetMgr()
	:m_Assets{}
{

}

AssetMgr::~AssetMgr()
{

}

void AssetMgr::Init()
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
	Sprite* _BackGroundSprite = Load<Sprite>(_path,L"BackGroundSprite");

	_path = L"\\resource\\NPC_Commander0.png";
	Sprite* _PlayerSprite = Load<Sprite>(_path, L"PlayerSprite");

	AddAsset(_BackGroundSprite, L"BackGroundSprite");
	AddAsset(_PlayerSprite, L"PlayerSprite");
	#pragma endregion

	#pragma region Mateiral
	Material* _defaultMaterial = new Material;
	
	AddAsset(_defaultMaterial, L"DefaultMat");
	#pragma endregion	
}
