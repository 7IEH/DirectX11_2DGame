#pragma once

class ConstantBuffer;

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

	ComPtr<ID3D11RasterizerState>			m_Rasterizer;

	HWND									m_hWnd;
	Vec2									m_vRenderResolution;

	UINT									m_4MSAAQuality;

	ConstantBuffer*							m_ConstantBuffer[(UINT)CONSTANT_TYPE::END];

public:
	void						ClearRenderTarget(float(&Color)[4]);
	void						Present();

	ID3D11Device*				GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext*		GetContext() { return m_DeviceContext.Get(); }

	Vec2						GetResolution() { return m_vRenderResolution; }
	HWND						GetHwnd() { return m_hWnd; }

	ConstantBuffer*				GetConstantBuffer(CONSTANT_TYPE _type) { return m_ConstantBuffer[(UINT)(CONSTANT_TYPE)_type]; }

public:
	int							Init(HWND _hWnd, Vec2 _vRenderResolution);

private:
	HRESULT						CreateDevice();
	HRESULT						CreateSwapChain();
	HRESULT						CreateRTView();
	HRESULT						CreateDSView();

	void						CreateViewPort();
	void						OMSetRT();

	void						CreateConstantBuffer(CONSTANT_TYPE _type, int _size, int _sizeCount);

	void						CreateRasterizerState();
};

