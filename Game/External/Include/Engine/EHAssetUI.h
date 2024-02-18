#pragma once
#include "EHUI.h"

#include "Ptr.h"
#include "EHAsset.h"

class AssetUI :
    public UI
{
private:
    Ptr<Asset>  m_Asset;
    ASSET_TYPE  m_Type;

public:
    Ptr<Asset>GetAsset() { return m_Asset; }
    void SetAsset(Ptr<Asset> _asset);

    ASSET_TYPE GetType() { return m_Type; }

public:
    virtual void Render_Update()override;

public:
    AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _type);
    virtual ~AssetUI();
};

