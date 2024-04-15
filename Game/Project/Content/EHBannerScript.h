#pragma once
#include <EHScript.h>
class BannerScript :
    public Script
{
private:
    GameObject* m_pBannerObject;
    float       m_fAccTime;

    bool        m_bFirstBanner;

public:
    virtual void Start()override;
    virtual void Update()override;
    

public:
    CLONE(BannerScript)
    BannerScript();
    virtual ~BannerScript();
};

