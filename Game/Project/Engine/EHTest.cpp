#include "pch.h"
#include "EHTest.h"

#include "EHDevice.h"

#include "EHMesh.h"
#include "EHShader.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"

Mesh* Nemo = new Mesh;
Shader* NemoShader = new Shader;

// ConstantBuffer
ComPtr<ID3D11Buffer> _constantBuffer;
tTransform pos;

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
	triangle[1]._UV = Vec2(0.f, 0.f);

	triangle[2]._Postion = Vec3(0.5f, -0.5f, 0.f);
	triangle[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[2]._UV = Vec2(0.f, 0.f);

	triangle[3]._Postion = Vec3(-0.5f, -0.5f, 0.f);
	triangle[3]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[3]._UV = Vec2(0.f, 0.f);
	// --------------------------------------------

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;

	idx[3] = 0;
	idx[4] = 2;
	idx[5] = 3;

	Nemo->Create(triangle, 4, idx, 6);

	// ConstantBuffer »ý¼º
	D3D11_BUFFER_DESC bDesc = {};

	bDesc.ByteWidth = sizeof(tTransform);
	bDesc.StructureByteStride = sizeof(tTransform);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&bDesc, nullptr, _constantBuffer.GetAddressOf())))
	{
		HandleError(_hWnd, L"Constant Create Error", 0);
		return;
	}

	wstring _path = L"\\shader\\std2d.fx";
	string _vsEntry = "VS_Std2D";
	string _psEntry = "PS_Std2D";

	NemoShader->Create(_path, _vsEntry, _psEntry);
}

void Test::Tick()
{
	if (KEY_PRESSED(KEY::A))
	{
		pos._Position.x -= DT;
	}

	if (KEY_PRESSED(KEY::D))
	{
		pos._Position.x += DT;
	}

	if (KEY_PRESSED(KEY::W))
	{
		pos._Position.y += DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		pos._Position.y -= DT;
	}

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	::memcpy(tSub.pData, &pos, sizeof(tTransform));
	CONTEXT->Unmap(_constantBuffer.Get(), 0);

	Nemo->UpdateData();
	CONTEXT->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	NemoShader->UpdateData();

	Nemo->Render();
}

void Test::Release()
{


}
