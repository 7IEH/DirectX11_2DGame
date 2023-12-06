#include "pch.h"
#include "EHEngine.h"

#include "EHTimeMgr.h"
#include "EHPathMgr.h"
#include "EHKeyMgr.h"

#include "EHDevice.h"

vtx triangle[3] = {};

ComPtr<ID3DBlob> _vsBlob;
ComPtr<ID3DBlob> _psBlob;
ComPtr<ID3DBlob> _errBlob;

ComPtr<ID3D11VertexShader> _VSShader;
ComPtr<ID3D11PixelShader> _PSShader;

ComPtr<ID3D11Buffer> _Buffer;

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
	triangle[0]._Postion = Vec3(0.f, 1.f, 0.f);
	triangle[0]._Color = Vec4(1.f, 0.f, 0.f, 1.f);
	triangle[0]._UV = Vec2(0.f, 0.f);

	triangle[1]._Postion = Vec3(1.f, -1.f, 0.f);
	triangle[1]._Color = Vec4(0.f, 1.f, 0.f, 1.f);
	triangle[1]._UV = Vec2(0.f, 0.f);

	triangle[2]._Postion = Vec3(-1.f, -1.f, 0.f);
	triangle[2]._Color = Vec4(0.f, 0.f, 1.f, 1.f);
	triangle[2]._UV = Vec2(0.f, 0.f);
	// --------------------------------------------


	D3D11_BUFFER_DESC bDesc = {};

	bDesc.ByteWidth = sizeof(vtx) * 3;
	bDesc.StructureByteStride = sizeof(vtx);
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = triangle;

	if (FAILED(DEVICE->CreateBuffer(&bDesc, &tSubData, _Buffer.GetAddressOf())))
	{
		HandleError(m_hWnd, L"Buffer Create Error", 0);
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

	float ClearColor[4] = { 1.f,1.f,1.f,1.f };
	Device::GetInst()->ClearRenderTarget(ClearColor);

	UINT _iStride = sizeof(vtx);
	UINT _iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, _Buffer.GetAddressOf(), &_iStride, &_iOffset);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(_Layout.Get());

	CONTEXT->VSSetShader(_VSShader.Get(), 0, 0);
	CONTEXT->PSSetShader(_PSShader.Get(), 0, 0);

	CONTEXT->Draw(3, 0);

	Device::GetInst()->Present();
}
