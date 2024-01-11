#pragma once
#include "EHAsset.h"

class Sprite
	:public Asset
{
private:
	ScratchImage									m_Image;
	ComPtr<ID3D11Texture2D>							m_SpriteTexture;
	D3D11_TEXTURE2D_DESC							m_DESC;

	ComPtr<ID3D11RenderTargetView>					m_RenderTargetView;
	ComPtr<ID3D11DepthStencilView>					m_DepthStencilView;
	ComPtr<ID3D11ShaderResourceView>				m_ShaderResourceView;
	ComPtr<ID3D11UnorderedAccessView>				m_UnorderedAccessView;

	Vec2											m_RT;
	Vec2											m_WH;

public:
	UINT									GetSpriteWidth() { return m_DESC.Width; }
	UINT									GetSpriteHeight() { return m_DESC.Height; }

	ComPtr<ID3D11RenderTargetView>			GetRTV()	{return m_RenderTargetView;	  }
	ComPtr<ID3D11DepthStencilView>			GetDSV()	{return m_DepthStencilView;	  }
	ComPtr<ID3D11ShaderResourceView>		GetSRV()	{return m_ShaderResourceView; }
	ComPtr<ID3D11UnorderedAccessView>		GetUAV()	{return m_UnorderedAccessView;}

	ID3D11Texture2D*						GetTexture2D() { return m_SpriteTexture.Get(); }

public:
	virtual void UpdateData()override {};
	virtual void Render()override {};
	void UpdateData(int _resgisterNumber);
	static void Clear(int _resgisterNumber);

private:
	virtual HRESULT Load(const wstring& _strFilePath)override;
	HRESULT Create(UINT _width, UINT _height,
		DXGI_FORMAT _format, UINT _bindflags,
		D3D11_USAGE _usage = D3D11_USAGE_DEFAULT);

public:
	Sprite();
	virtual ~Sprite();

	friend class AssetMgr;
};

