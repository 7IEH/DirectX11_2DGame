#pragma once
#include "EHEntity.h"

class Asset :
    public Entity
{
private:
    ASSET_TYPE m_Type;

public:
    ASSET_TYPE GetType() { return m_Type; }

public:
    virtual void UpdateData() = 0;
    virtual void Render() = 0;

private:
    virtual HRESULT Load(const wstring& _strFilePath) { return E_FAIL; };

public:
    Asset(ASSET_TYPE _type);
    virtual ~Asset();

    friend class AssetMgr;
};

