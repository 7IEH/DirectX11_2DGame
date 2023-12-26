#pragma once
#include "EHEntity.h"

class Asset :
    public Entity
{
private:
    wstring         m_Key;
    wstring         m_RelativeFilePath;
    ASSET_TYPE      m_Type;

public:
    ASSET_TYPE GetType() { return m_Type; }
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativeFilePath; }

private:
    void SetKey(const wstring& _key) { m_Key = _key; }
    void SetRelativePath(const wstring& _relativeFilePath) { m_RelativeFilePath = _relativeFilePath; }

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

