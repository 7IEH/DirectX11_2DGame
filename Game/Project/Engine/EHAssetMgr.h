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

public:
	void Init();
};

