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
	UINT GetSpriteWidth() { return m_DESC.Width; }
	UINT GetSpriteHeight() { return m_DESC.Height; }

public:
	virtual void UpdateData()override {};
	virtual void Render()override {};
	void UpdateData(int _resgisterNumber);
	static void Clear(int _resgisterNumber);

private:
	virtual HRESULT Load(const wstring& _strFilePath)override;

public:
	Sprite();
	virtual ~Sprite();

	friend class AssetMgr;
};

