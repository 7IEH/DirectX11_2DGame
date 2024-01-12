#include "pch.h"
#include "EHAssetMgr.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHSprite.h"
#include "EHMaterial.h"

#include "EHDebugMgr.h"

Ptr<Sprite> AssetMgr::CreateResoruceTexture(const wstring& _label, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _flag, D3D11_USAGE _usage)
{
	Ptr<Sprite> _sprite = FindAsset<Sprite>(_label);
	assert(!_sprite.Get());

	_sprite = new Sprite;

	HRESULT _hr = _sprite->Create(_width, _height, _format, _flag, _usage);

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"RESOURCE TEXTURE CREATE FAIELD!", 1);
		return nullptr;
	}

	AddAsset<Sprite>(_sprite.Get(), _label);

	return _sprite;
}

Ptr<Sprite> AssetMgr::CreateResoruceTexture(const wstring& _label, ComPtr<ID3D11Texture2D> _tex2D)
{
	// 생성하려는 텍스쳐와 동일한 키의 텍스쳐가 이미 AssetMgr 에 있다면
	Ptr<Sprite> _sprite = FindAsset<Sprite>(_label);
	assert(!_sprite.Get());

	_sprite = new Sprite;

	HRESULT _hr = _sprite->Create(_tex2D);

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"RESOURCE TEXTURE CREATE FAIELD!", 1);
		return nullptr;
	}

	AddAsset<Sprite>(_sprite.Get(), _label);

	return _sprite;
}