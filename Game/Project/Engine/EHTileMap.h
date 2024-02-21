#pragma once
#include "EHRenderer.h"

class StructuredBuffer;

struct tTileInfo
{
    Vec2 vLeftTopUV;
    int  bRender;
    int  padding;
};


class TileMap :
    public Renderer
{
private:
    UINT                m_FaceX;            // 가로 타일 개수
    UINT                m_FaceY;            // 세로 타일 개수
    Vec2                m_vTileRenderSize;  // 타일 1칸 사이즈


    Ptr<Sprite>         m_TileAtlas;
    Vec2                m_vTilePixelSize;
    Vec2                m_vSliceSizeUV;

    UINT                m_MaxCol;
    UINT                m_MaxRow;

    UINT                m_TileIdx;

    vector<tTileInfo>   m_vecTileInfo;

    StructuredBuffer* m_TileInfoBuffer;

public:
    void SetTileAtlas(Ptr<Sprite> _Atlas, Vec2 _TilePixelSize);
    Ptr<Sprite> GetTileAtlas() { return m_TileAtlas; }

    void SetFace(UINT _FaceX, UINT _FaceY);
    UINT GetFaceX() { return m_FaceX; }
    UINT GetFaceY() { return m_FaceY; }

    Vec2            GetRenderTileSize() { return m_vTileRenderSize; }
    void            SetRenderTileSize(Vec2 _vTileRenderSize) { m_vTileRenderSize = _vTileRenderSize; }

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx, int _render);

    CLONE(TileMap)

public:
    virtual void LateUpdate() override;
    virtual void UpdateData() override;
    virtual void Render() override;

    virtual void Save(string _path)override;

public:
    TileMap();
    TileMap(const TileMap& _origin);
    ~TileMap();
};

