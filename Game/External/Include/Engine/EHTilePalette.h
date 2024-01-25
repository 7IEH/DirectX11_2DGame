#pragma once
#include "EHUI.h"
#include "EHSprite.h"

struct tSelectedTile
{
    ImVec2 _stUV;
    ImVec2 _enUV;
    
    UINT    _tileIdx;
    int     _bRender;

    int     _Collider;
};

class TilePalette :
    public UI
{
private:
    float                       m_buttonWidth;

    string                      m_CurrentLabel;
    vector<tTilePaletteInfo>    m_TileInfo;


    string                      m_TileName;
    int                         m_Face[2];
    float                       m_RenderSize[2];
    float                       m_PixelSize[2];
    float                       m_prevFace[2];

    tSelectedTile               m_ClickedTile;

public:
    virtual void Render_Update()override;

private:
    void Canvas(Ptr<Sprite> _atlasImage);
    void Setting(Ptr<Sprite> _atlasImage);
    void Palette(Ptr<Sprite> _atlasImage);

public:
    TilePalette();
    virtual ~TilePalette();
};

