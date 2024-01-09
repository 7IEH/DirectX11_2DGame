#include "pch.h"
#include "EHDevice.h"

#include "EHConstantBuffer.h"

Device::Device()
	: m_hWnd(nullptr)
	, m_4MSAAQuality(0)
	, m_vRenderResolution{}
	, m_ConstantBuffer{}
{
}

Device::~Device()
{
	ReleaseArray(m_ConstantBuffer);
}

int Device::Awake(HWND _hWnd, Vec2 _vRenderResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vRenderResolution;

	// Device Create
	if (FAILED(CreateDevice()))
	{
		return E_FAIL;
	}


	// MultiSampling check
	DEVICE->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4MSAAQuality);
	assert(m_4MSAAQuality > 0);

	// SwapChain Create
	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}

	// RTVIEW Create
	if (FAILED(CreateRTView()))
	{
		return E_FAIL;
	}

	// DSVIEW Create
	if (FAILED(CreateDSView()))
	{
		return E_FAIL;
	}

	// 입출력 병합 단계
	// RenderTarget , DSView Set
	OMSetRT();

	// ViewPort Create
	CreateViewPort();

	// RasterizeState Create
	if (FAILED(CreateRasterizerState()))
	{
		return E_FAIL;
	}

	// DepthStencil
	if (FAILED(CreateDepthStencilState()))
	{
		return E_FAIL;
	}

	// BlendState
	if (FAILED(CreateBlendState()))
	{
		return E_FAIL;
	}

	// SamplerState
	if (FAILED(CreateSamplerState()))
	{
		return E_FAIL;
	}

	// Create Transform, Material, Light, Normal Vector
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateDevice()
{
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, m_Device.GetAddressOf(),
		&eLevel, m_DeviceContext.GetAddressOf())))
	{
		HandleError(m_hWnd, L"Device Create Failed!", 1);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDesc = {};
	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.Flags = 0;

	// 현재는 꺼둠
	// MultiSampling check
	if (true)
	{
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
	}
	else
	{
		tDesc.SampleDesc.Count = 4;
		tDesc.SampleDesc.Quality = m_4MSAAQuality - 1;
	}

	tDesc.Windowed = true;
	tDesc.OutputWindow = m_hWnd;

	ComPtr<IDXGIDevice>			pDevice = nullptr;
	ComPtr<IDXGIAdapter>		pAdapter = nullptr;
	ComPtr<IDXGIFactory>		pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDevice.GetAddressOf());
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
	{
		HandleError(m_hWnd, L"SwapChain Create Failed!", 1);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateRTView()
{
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTexture.GetAddressOf());
	m_Device->CreateRenderTargetView(m_RTTexture.Get(), nullptr, m_RTView.GetAddressOf());
	return S_OK;
}

HRESULT Device::CreateDSView()
{
	D3D11_TEXTURE2D_DESC Desc = {};

	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Desc.Width = (UINT)m_vRenderResolution.x;
	Desc.Height = (UINT)m_vRenderResolution.y;
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	Desc.MipLevels = 1;
	Desc.MiscFlags = 0;

	Desc.ArraySize = 1;

	if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTexture.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_Device->CreateDepthStencilView(m_DSTexture.Get(), nullptr, m_DSView.GetAddressOf());

	return S_OK;
}

void Device::OMSetRT()
{
	// 최대 8개의 렌더타켓 가능
	m_DeviceContext->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());
}

HRESULT Device::CreateConstantBuffer()
{
	HRESULT _hr = CreateConstantBufferIndividual(CONSTANT_TYPE::TRANSFORM, sizeof(transform), 1);
	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Device Class Transform ConstantBuffer Initialize Failed!", 0);
		return E_FAIL;
	}

	_hr = CreateConstantBufferIndividual(CONSTANT_TYPE::MATERIAL, sizeof(material), 1);
	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Device Class Material ConstantBuffer Initialize Failed!", 0);
		return E_FAIL;
	}

	_hr = CreateConstantBufferIndividual(CONSTANT_TYPE::NORMANL, sizeof(NomralVector), 1);
	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Device Class Normal ConstantBuffer Initialize Failed!", 0);
		return E_FAIL;
	}

	_hr = CreateConstantBufferIndividual(CONSTANT_TYPE::ANIMATION, sizeof(tAnimationInfo), 1);
	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Device Class Animation ConstantBuffer Initialize Failed!", 0);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateConstantBufferIndividual(CONSTANT_TYPE _type, UINT _elementSize, UINT _elementCount)
{
	m_ConstantBuffer[(UINT)_type] = new ConstantBuffer;
	HRESULT _hr = m_ConstantBuffer[(UINT)_type]->Create(_elementSize, _elementCount, _type);

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Device Class ConstantBuffer Initialize Failed!", 0);
		return E_FAIL;
	}
	return S_OK;
}

void Device::ClearRenderTarget(float(&Color)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RTView.Get(), Color);
	m_DeviceContext->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Device::Present()
{
	m_SwapChain->Present(0, 0);
}

void Device::CreateViewPort()
{
	D3D11_VIEWPORT vDesc = {};

	vDesc.MinDepth = 0;
	vDesc.MaxDepth = 1.f;

	vDesc.TopLeftX = 0;
	vDesc.TopLeftY = 0;
	vDesc.Width = m_vRenderResolution.x;
	vDesc.Height = m_vRenderResolution.y;

	m_DeviceContext->RSSetViewports(1, &vDesc);
}

HRESULT Device::CreateRasterizerState()
{
	m_Rasterizer[(UINT)CULL_TYPE::BACK] = nullptr;

	D3D11_RASTERIZER_DESC tDesc = {};
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_FRONT;

	tDesc.FrontCounterClockwise = FALSE;
	tDesc.DepthClipEnable = TRUE;
	tDesc.ScissorEnable = FALSE;
	tDesc.MultisampleEnable = FALSE;
	tDesc.AntialiasedLineEnable = FALSE;

	HRESULT _hr = DEVICE->CreateRasterizerState(&tDesc, m_Rasterizer[(UINT)CULL_TYPE::FRONT].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Rasterizer Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_NONE;

	tDesc.FrontCounterClockwise = FALSE;
	tDesc.DepthClipEnable = TRUE;
	tDesc.ScissorEnable = FALSE;
	tDesc.MultisampleEnable = FALSE;
	tDesc.AntialiasedLineEnable = FALSE;

	_hr = DEVICE->CreateRasterizerState(&tDesc, m_Rasterizer[(UINT)CULL_TYPE::NONE].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Rasterizer Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	tDesc.CullMode = D3D11_CULL_NONE;

	tDesc.FrontCounterClockwise = FALSE;
	tDesc.DepthClipEnable = TRUE;
	tDesc.ScissorEnable = FALSE;
	tDesc.MultisampleEnable = FALSE;
	tDesc.AntialiasedLineEnable = FALSE;

	_hr = DEVICE->CreateRasterizerState(&tDesc, m_Rasterizer[(UINT)CULL_TYPE::NONE].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Rasterizer Initialized Failed!", 0);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateDepthStencilState()
{
	m_DepthStencil[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	tDesc.DepthEnable = TRUE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	tDesc.StencilEnable = FALSE;

	HRESULT _hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};

	tDesc.DepthEnable = TRUE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	tDesc.StencilEnable = FALSE;

	_hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::GREATER].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};

	tDesc.DepthEnable = TRUE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	tDesc.StencilEnable = FALSE;

	_hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};

	tDesc.DepthEnable = FALSE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.StencilEnable = FALSE;

	_hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};

	tDesc.DepthEnable = TRUE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	tDesc.StencilEnable = FALSE;

	_hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::N0_WRITE].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	tDesc = {};

	tDesc.DepthEnable = FALSE;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.StencilEnable = FALSE;

	_hr = DEVICE->CreateDepthStencilState(&tDesc, m_DepthStencil[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"DepthStencilState Initialized Failed!", 0);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateBlendState()
{
	m_Blend[(UINT)BLEND_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc = {};
	tDesc.AlphaToCoverageEnable = FALSE;
	tDesc.IndependentBlendEnable = TRUE;

	tDesc.RenderTarget[0].BlendEnable = TRUE;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT _hr = DEVICE->CreateBlendState(&tDesc, m_Blend[(UINT)BLEND_TYPE::ALPHABLENDING].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"BlendState Initialized Failed!", 0);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Device::CreateSamplerState()
{
	m_Sampler[(UINT)SAMPLER_TYPE::Default] = nullptr;

	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.MipLODBias = 0.0f;
	tDesc.MaxAnisotropy = 1;
	tDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	tDesc.BorderColor[0] = 0;
	tDesc.BorderColor[1] = 0;
	tDesc.BorderColor[2] = 0;
	tDesc.BorderColor[3] = 0;
	tDesc.MinLOD = 0;
	tDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT _hr = DEVICE->CreateSamplerState(&tDesc, m_Sampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"SamplerState Initialized Failed!", 0);
		return E_FAIL;
	}

	SetSamplerState();

	return S_OK;
}

void Device::SetSamplerState()
{
	for (UINT i = 0;i < (UINT)SAMPLER_TYPE::END;i++)
	{
		CONTEXT->VSSetSamplers(i, 1, m_Sampler[i].GetAddressOf());
		CONTEXT->PSSetSamplers(i, 1, m_Sampler[i].GetAddressOf());
		CONTEXT->HSSetSamplers(i, 1, m_Sampler[i].GetAddressOf());
		CONTEXT->DSSetSamplers(i, 1, m_Sampler[i].GetAddressOf());
		CONTEXT->GSSetSamplers(i, 1, m_Sampler[i].GetAddressOf());
	}
}