#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
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
	Mesh* _mesh = new Mesh;
	GraphicShader* _shader = new GraphicShader;

	Mesh* _texture_mesh = new Mesh;
	GraphicShader* _texture_shader = new GraphicShader;

	Material* _defaultMaterial = new Material;

	vtx triangle[4] = {};
	UINT idx[6] = {};
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

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	_shader->Create(_path, _vsEntry, _psEntry);

	_path = L"\\resource\\Example.png";
	_shader->CreateResourceView(_path);
	_shader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_mesh, L"BackGroundMesh");
	AddAsset(_shader, L"BackGroundShader");

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

	_path = L"\\shader\\std2d.fx";
	_vsEntry = "VS_Std2D";
	_psEntry = "PS_Std2D";

	_texture_shader->Create(_path, _vsEntry, _psEntry);

	_path = L"\\resource\\NPC_Commander0.png";
	_texture_shader->CreateResourceView(_path);

	_texture_shader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	AddAsset(_texture_mesh, L"PlayerMesh");
	AddAsset(_texture_shader, L"PlayerShader");
	AddAsset(_defaultMaterial, L"DefaultMat");
}
