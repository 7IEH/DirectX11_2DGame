#pragma once
#include "EHAsset.h"

class Shader :
    public Asset
{ 
protected:
    ComPtr<ID3DBlob> m_ErrBlob;

public:
    virtual HRESULT UpdateData() = 0;

public:
    Shader(ASSET_TYPE _type);
    virtual ~Shader();
};

