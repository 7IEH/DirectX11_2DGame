#pragma once

#include "EHDevice.h"

#include "EHMesh.h"
#include "EHGraphicShader.h"
#include "EHMaterial.h"
#include "EHSprite.h"


class AssetMgr
	:public Singleton<AssetMgr>
{
	Single(AssetMgr);
private:
	map<wstring, Ptr<Asset>>	m_Assets[(UINT)ASSET_TYPE::END];

public:
	template <typename T>
	void AddAsset(T* _asset, const wstring& _name);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _name);

	template<typename T>
	Ptr<T> Load(const wstring& _strFilePath, const wstring _name);

private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();

public:
	void Awake();
};

template<typename T>
ASSET_TYPE GetAssetType()
{
	// info 정보 안에 해당 클래스에 대한 정보가 들어있음
	const type_info& info = typeid(T);

	ASSET_TYPE _type = ASSET_TYPE::END;

	if (&info == &typeid(Mesh))
		_type = ASSET_TYPE::MESH;
	else if (&info == &typeid(Sprite))
		_type = ASSET_TYPE::SPRITE;
	else if (&info == &typeid(GraphicShader))
		_type = ASSET_TYPE::GRAPHIC_SHADER;
	else if (&info == &typeid(Material))
		_type = ASSET_TYPE::MATERIAL;
	/*else if (&info == &typeid(Sound))
		_type = ASSET_TYPE::SOUND;*/

	return _type;
}

template<typename T>
inline void AssetMgr::AddAsset(T* _asset, const wstring& _name)
{
	ASSET_TYPE _type = GetAssetType<T>();

	map<wstring, Ptr<Asset>>::iterator iter = m_Assets[(UINT)_type].find(_name);
	assert(iter == m_Assets[(UINT)_type].end());

	m_Assets[(UINT)_type].insert(make_pair(_name,_asset));
}

template<typename T>
Ptr<T> AssetMgr::FindAsset(const wstring& _name)
{
	ASSET_TYPE _type = GetAssetType<T>();

	map<wstring, Ptr<Asset>>::iterator iter = m_Assets[(UINT)_type].find(_name);
	
	// check for exist
	if (iter == m_Assets[(UINT)_type].end())
	{
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template<typename T>
Ptr<T> AssetMgr::Load(const wstring& _strFilePath, const wstring _name)
{
	Ptr<T> _asset = FindAsset<T>(_name);

	if (_asset != nullptr)
	{
		return (T*)_asset.Get();
	}

	_asset = new T;
	if (FAILED(_asset->Load(_strFilePath)))
	{
		HandleError(MAIN_HWND, L"에셋 로딩 실패!", 0);
		_asset = nullptr;
		return nullptr;
	}

	_asset->SetKey(_name);
	_asset->SetRelativePath(_strFilePath);

	AddAsset<T>((T*)_asset.Get(), _name);

	return (T*)_asset.Get();
}