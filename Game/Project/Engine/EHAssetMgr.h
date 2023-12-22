#pragma once

class Asset;
class AssetMgr
	:public Singleton<AssetMgr>
{
	Single(AssetMgr);
private:
	map<wstring, Asset*>m_Assets;

public:
	template <typename T>
	void AddAsset(T* _asset, wstring _name)
	{
		map<wstring, Asset*>::iterator iter = m_Assets.find(_name);
		if (iter != m_Assets.end())
			return;
		m_Assets.insert({ _name,_asset });
	}

	Asset* FindAsset(const wstring _name)
	{
		map<wstring, Asset*>::iterator iter = m_Assets.find(_name);
		if (iter == m_Assets.end())
			return nullptr;
		return (*iter).second;
	}

	template<typename T>
	T* Load(wstring& _strFilePath,wstring _name)
	{
		map<wstring, Asset*>::iterator iter = m_Assets.find(_name);
		if (iter != m_Assets.end())
			return dynamic_cast<T*>((*iter).second);

		T* _temp = new T;
		if (_temp->GetType() == ASSET_TYPE::SPRITE)
		{
			_temp->Load(_strFilePath);
		}

		return _temp;
	}

public:
	void Init();
};

