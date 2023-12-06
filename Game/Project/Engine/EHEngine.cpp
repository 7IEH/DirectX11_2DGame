#include "pch.h"
#include "EHEngine.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"

#include "EHDevice.h"

vtx triangle[4] = {};

ComPtr<ID3DBlob> _vsBlob;
ComPtr<ID3DBlob> _psBlob;
ComPtr<ID3DBlob> _errBlob;

ComPtr<ID3D11VertexShader> _VSShader;
ComPtr<ID3D11PixelShader> _PSShader;

// VertexBuffer
ComPtr<ID3D11Buffer> _vertexBuffer;

// IndexBuffer
ComPtr<ID3D11Buffer> _indexBuffer;
UINT idx[6];

// ConstantBuffer
ComPtr<ID3D11Buffer> _constantBuffer;
tTransform pos;


ComPtr<ID3D11InputLayout> _Layout;

Engine::Engine()
	:m_vResolution{}
	, m_hWnd(nullptr)
{

}

Engine::~Engine()
{

}

int Engine::Init(Vec2 _vResolution, HWND _hWnd)
{
	// Window Setting
	m_hWnd = _hWnd;
	m_vResolution = _vResolution;

	RECT rt = { 0,0,(int)m_vResolution.x,(int)m_vResolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10.f, 10.f, rt.right - rt.left, rt.bottom - rt.top, 0);

	// Manger Initialize
	TimeMgr::GetInst()->Init();
	PathMgr::GetInst()->Init();
	KeyMgr::GetInst()->Init();

	// Device Initialize
	if (FAILED(Device::GetInst()->Init(m_hWnd, m_vResolution)))
	{
		HandleError(m_hWnd, L"Device Initialize Failed!", 1);
		return E_FAIL;
	}

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

	// VertexBuffer 생성
	D3D11_BUFFER_DESC bDesc = {};

	bDesc.ByteWidth = sizeof(vtx) * 4;
	bDesc.StructureByteStride = sizeof(vtx);
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = triangle;

	if (FAILED(DEVICE->CreateBuffer(&bDesc, &tSubData, _vertexBuffer.GetAddressOf())))
	{
		HandleError(m_hWnd, L"vertexBuffer Create Error", 0);
		return E_FAIL;
	}

	// IndexBuffer 생성
	bDesc = {};

	bDesc.ByteWidth = sizeof(UINT) * 6;
	bDesc.StructureByteStride = sizeof(UINT);
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	bDesc.CPUAccessFlags = 0;
	bDesc.Usage = D3D11_USAGE_DEFAULT;

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;

	idx[3] = 0;
	idx[4] = 2;
	idx[5] = 3;

	tSubData = {};
	tSubData.pSysMem = idx;

	if (FAILED(DEVICE->CreateBuffer(&bDesc, &tSubData, _indexBuffer.GetAddressOf())))
	{
		HandleError(m_hWnd, L"IndexBuffer Create Error", 0);
		return E_FAIL;
	}

	// ConstantBuffer 생성
	// IndexBuffer 생성
	bDesc = {};

	bDesc.ByteWidth = sizeof(tTransform);
	bDesc.StructureByteStride = sizeof(tTransform);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&bDesc, nullptr, _constantBuffer.GetAddressOf())))
	{
		HandleError(m_hWnd, L"IndexBuffer Create Error", 0);
		return E_FAIL;
	}	
	
	// 버텍스 쉐이더
	wstring strFilePath = PATH;

	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"\\shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Std2D", "vs_5_0", D3DCOMPILE_DEBUG, 0
		, _vsBlob.GetAddressOf(), _errBlob.GetAddressOf())))
	{
		HandleError(m_hWnd, L"VSBLOB CREATE ERROR", 0);
		return E_FAIL;
	}

	DEVICE->CreateVertexShader(_vsBlob->GetBufferPointer()
		, _vsBlob->GetBufferSize(), nullptr
		, _VSShader.GetAddressOf());

	// 정점 구조정보(Layout) 생성
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	DEVICE->CreateInputLayout(arrElement, 3, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _Layout.GetAddressOf());

	// 픽셀 쉐이더
	strFilePath = PATH;

	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"\\shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Std2D", "ps_5_0", D3DCOMPILE_DEBUG, 0
		, _psBlob.GetAddressOf(), _errBlob.GetAddressOf())))
	{
		HandleError(m_hWnd, L"PSBLOB CREATE ERROR", 0);
		return E_FAIL;
	}

	DEVICE->CreatePixelShader(_psBlob->GetBufferPointer()
		, _psBlob->GetBufferSize(), nullptr
		, _PSShader.GetAddressOf());


	return S_OK;
}

void Engine::Progress()
{
	//Manger Progress
	TimeMgr::GetInst()->Progress();
	KeyMgr::GetInst()->Tick();

	float ClearColor[4] = { 1.f,1.f,1.f,1.f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

	UINT _iStride = sizeof(vtx);
	UINT _iOffset = 0;
	
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	
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

	CONTEXT->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	::memcpy(tSub.pData, &pos, sizeof(tTransform));
	CONTEXT->Unmap(_constantBuffer.Get(), 0);


	CONTEXT->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &_iStride, &_iOffset);
	CONTEXT->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(_Layout.Get());

	CONTEXT->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	CONTEXT->VSSetShader(_VSShader.Get(), 0, 0);
	CONTEXT->PSSetShader(_PSShader.Get(), 0, 0);

	CONTEXT->DrawIndexed(6, 0, 0);

	Device::GetInst()->Present();
}
