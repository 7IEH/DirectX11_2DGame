#include "pch.h"
#include "EHTest.h"

#include "EHDevice.h"

// Resource
#include "EHMesh.h"
#include "EHShader.h"
#include "EHGraphicShader.h"

// Component
#include "EHTransform.h"
#include "EHMeshRenderer.h"

// Mgr
#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"

// Object
#include "EHPlayer.h"

Mesh* Nemo = new Mesh;
GraphicShader* NemoShader = new GraphicShader;

// ConstantBuffer
ComPtr<ID3D11Buffer> _constantBuffer;

// Test Player
Player* _player = new Player;

void Test::Init(HWND _hWnd)
{
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

	Nemo->Create(triangle, 4, idx, 6);

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	NemoShader->Create(_path, _vsEntry, _psEntry);

	_path = L"\\resource\\NPC_Commander0.png";
	NemoShader->CreateResourceView(_path);

	_player->Init();
	_player->AddComponent<Transform>();
	MeshRenderer* _playerRenderer = _player->AddComponent<MeshRenderer>();
	_playerRenderer->SetMesh(Nemo);
	_playerRenderer->SetShader(NemoShader);
}

void Test::Tick()
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	_player->Tick();
}

void Test::Render()
{
	_player->Render();
}

void Test::Release()
{
}
