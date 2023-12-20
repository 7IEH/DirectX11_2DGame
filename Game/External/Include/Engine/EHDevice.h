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

	ComPtr<ID3D11RasterizerState>			m_Rasterizer[(UINT)CULL_TYPE::END];
	ComPtr<ID3D11DepthStencilState>			m_DepthStencil[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>				m_Blend[(UINT)BLEND_TYPE::END];
	ComPtr<ID3D11SamplerState>				m_Sampler[(UINT)SAMPLER_TYPE::END];

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

	ComPtr<ID3D11RasterizerState> GetRSState(CULL_TYPE _Type) { return m_Rasterizer[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DepthStencil[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BLEND_TYPE _Type) { return m_Blend[(UINT)_Type]; }
	ComPtr<ID3D11SamplerState> GetSamplerState(SAMPLER_TYPE _Type) { return m_Sampler[(UINT)_Type]; }

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
	void						CreateDepthStencilState();
	void						CreateBlendState();
	void						CreateSamplerState();
};

