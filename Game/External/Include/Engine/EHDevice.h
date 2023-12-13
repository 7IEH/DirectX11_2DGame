#pragma once

class Device
	: public Singleton<Device>
{
	Single(Device)
private:
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;

	ComPtr<IDXGISwapChain>					m_SwapChain;

	// RenderTarget Texture , RenderTarget View
	ComPtr<ID3D11Texture2D>					m_RTTexture;
	ComPtr<ID3D11RenderTargetView>			m_RTView;

	// Depth-Stencil Texture, Depth-Stencil View
	ComPtr<ID3D11Texture2D>					m_DSTexture;
	ComPtr<ID3D11DepthStencilView>			m_DSView;

	HWND									m_hWnd;
	Vec2									m_vRenderResolution;

	UINT									m_4MSAAQuality;

private:
	HRESULT CreateDevice();
	HRESULT CreateSwapChain();
	HRESULT CreateRTView();
	HRESULT CreateDSView();
	void CreateViewPort();
	
	void OMSetRT();

public:
	void ClearRenderTarget(float(&Color)[4]);
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_DeviceContext.Get(); }

public:
	int Init(HWND _hWnd, Vec2 _vRenderResolution);
};

