#include "pch.h"
#include "EHSprite.h"

#include "EHDevice.h"
#include "EHPathMgr.h"

Sprite::Sprite()
	:Asset(ASSET_TYPE::SPRITE)
	, m_DESC{}
{
}

Sprite::~Sprite()
{
}

HRESULT Sprite::Load(const wstring& _strFilePath)
{
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	wstring _FinalPath = PATH + _strFilePath;

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		hr = LoadFromDDSFile(_FinalPath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(_FinalPath.c_str(), nullptr, m_Image);
	}

	else
	{
		// png, bmp, jpg, jpeg
		TexMetadata* temp = nullptr;
		hr = LoadFromWICFile(_FinalPath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), m_ShaderResourceView.GetAddressOf());
	m_ShaderResourceView.Get()->GetResource((ID3D11Resource**)m_SpriteTexture.GetAddressOf());
	m_SpriteTexture.Get()->GetDesc(&m_DESC);

	return S_OK;
}

void Sprite::UpdateData(int _resgisterNumber)
{
	CONTEXT->VSSetShaderResources(_resgisterNumber, 1, m_ShaderResourceView.GetAddressOf());
	CONTEXT->PSSetShaderResources(_resgisterNumber, 1, m_ShaderResourceView.GetAddressOf());
	CONTEXT->HSSetShaderResources(_resgisterNumber, 1, m_ShaderResourceView.GetAddressOf());
	CONTEXT->DSSetShaderResources(_resgisterNumber, 1, m_ShaderResourceView.GetAddressOf());
	CONTEXT->GSSetShaderResources(_resgisterNumber, 1, m_ShaderResourceView.GetAddressOf());
}

void Sprite::Clear(int _resgisterNumber)
{
	ID3D11ShaderResourceView* _clearSRV = nullptr;

	CONTEXT->VSSetShaderResources(_resgisterNumber, 1, &_clearSRV);
	CONTEXT->PSSetShaderResources(_resgisterNumber, 1, &_clearSRV);
	CONTEXT->HSSetShaderResources(_resgisterNumber, 1, &_clearSRV);
	CONTEXT->DSSetShaderResources(_resgisterNumber, 1, &_clearSRV);
	CONTEXT->GSSetShaderResources(_resgisterNumber, 1, &_clearSRV);
}

HRESULT Sprite::Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindflags, D3D11_USAGE _usage)
{
	m_DESC.Format = _format;
	m_DESC.BindFlags = _bindflags;

	m_DESC.Width = _width;
	m_DESC.Height = _height;

	m_DESC.Usage = _usage;
	if (_usage == D3D11_USAGE_DYNAMIC)
	{
		m_DESC.Usage = D3D11_USAGE_DYNAMIC;
	}

	m_DESC.SampleDesc.Count = 1;
	m_DESC.SampleDesc.Quality = 0;

	m_DESC.MipLevels = 1;
	m_DESC.MiscFlags = 0;
	m_DESC.ArraySize = 1;

	HRESULT _hr = DEVICE->CreateTexture2D(&m_DESC, nullptr, m_SpriteTexture.GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"Texture DX Resource Create Failed!", 0);
		return E_FAIL;
	}

	if (m_DESC.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		_hr = DEVICE->CreateDepthStencilView(m_SpriteTexture.Get(), nullptr, m_DepthStencilView.GetAddressOf());
		if (FAILED(_hr))
		{
			HandleError(MAIN_HWND, L"Texture DX DepthStencil View Create Failed!", 0);
			return E_FAIL;
		}
	}
	else
	{
		if (m_DESC.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			_hr = DEVICE->CreateRenderTargetView(m_SpriteTexture.Get(), nullptr, m_RenderTargetView.GetAddressOf());
			if (FAILED(_hr))
			{
				HandleError(MAIN_HWND, L"Texture DX RenderTarget View Create Failed!", 0);
				return E_FAIL;
			}
		}

		if (m_DESC.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			_hr = DEVICE->CreateShaderResourceView(m_SpriteTexture.Get(), nullptr, m_ShaderResourceView.GetAddressOf());
			if (FAILED(_hr))
			{
				HandleError(MAIN_HWND, L"Texture DX Shaderresource View Create Failed!", 0);
				return E_FAIL;
			}
		}

		if (m_DESC.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			_hr = DEVICE->CreateUnorderedAccessView(m_SpriteTexture.Get(), nullptr, m_UnorderedAccessView.GetAddressOf());
			if (FAILED(_hr))
			{
				HandleError(MAIN_HWND, L"Texture DX UnorderedAccess View Create Failed!", 0);
				return E_FAIL;
			}
		}
	}


	return S_OK;
}