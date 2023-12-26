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