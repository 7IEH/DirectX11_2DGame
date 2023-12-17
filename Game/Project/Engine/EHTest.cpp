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



// Test Player
GameObject* _player = new GameObject;
Player* _pla2 = new Player;

void Test::Init(HWND _hWnd)
{
	
}

void Test::Tick()
{
	_player->Tick();
	_pla2->Tick();
}

void Test::Render()
{
	_player->Render();
	_pla2->Render();
}

void Test::Release()
{
	delete _player;
	delete _pla2;
}
