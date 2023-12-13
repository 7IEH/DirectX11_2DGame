#include "pch.h"
#include "EHDevice.h"

Device::Device()
	:
	m_Device(nullptr)
	, m_DeviceContext(nullptr)
	, m_SwapChain(nullptr)
	, m_RTTexture(nullptr)
	, m_RTView(nullptr)
	, m_DSTexture(nullptr)
	, m_DSView(nullptr)
	, m_hWnd(nullptr)
	, m_vRenderResolution{}
	, m_4MSAAQuality(0)
{
}

Device::~Device()
{

}

int Device::Init(HWND _hWnd, Vec2 _vRenderResolution)
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
		HandleError(m_hWnd, L"SwapChain Create Failed!",1);
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