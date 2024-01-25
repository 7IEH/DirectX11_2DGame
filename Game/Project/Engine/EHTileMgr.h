#pragma once

class TileMgr
	:public Singleton<TileMgr>
{
	Single(TileMgr)
private:
	map<wstring,vector<tTilePaletteInfo>> m_TileInfo;

public:
	vector<tTilePaletteInfo> FindTileInfo(const wstring& _label);
	vector<tTilePaletteInfo> GetTileInfo(const wstring& _label);
	void AddTileInfo(const wstring& _label, vector<tTilePaletteInfo>& _tileInfo);

	map<wstring, vector<tTilePaletteInfo>>& GetTileAllInfo() { return m_TileInfo; }
};

