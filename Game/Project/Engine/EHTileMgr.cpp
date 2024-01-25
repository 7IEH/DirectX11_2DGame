#include "pch.h"
#include "EHTileMgr.h"

TileMgr::TileMgr()
	:m_TileInfo{}
{
}

TileMgr::~TileMgr()
{
}

vector<tTilePaletteInfo> TileMgr::FindTileInfo(const wstring& _label)
{
	vector<tTilePaletteInfo> _info = {};

	map<wstring, vector<tTilePaletteInfo>>::iterator iter = m_TileInfo.find(_label);
	
	if (iter == m_TileInfo.end())
	{
		return _info;
	}

	_info = iter->second;

	return _info;
}

vector<tTilePaletteInfo> TileMgr::GetTileInfo(const wstring& _label)
{
	vector<tTilePaletteInfo> _info = FindTileInfo(_label);

	if (_info.size() == 0)
	{
		return _info;
	}
	return _info;
}

void TileMgr::AddTileInfo(const wstring& _label, vector<tTilePaletteInfo>& _tileInfo)
{
	vector<tTilePaletteInfo> _info = FindTileInfo(_label);

	if (_info.size() != 0)
		return;

	m_TileInfo.insert(make_pair(_label, _tileInfo));
}
