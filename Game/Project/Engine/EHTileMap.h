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
    UINT                m_FaceX;            // ���� Ÿ�� ����
    UINT                m_FaceY;            // ���� Ÿ�� ����
    Vec2                m_vTileRenderSize;  // Ÿ�� 1ĭ ������


    Vec3                m_vTileMapWorldPos;

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

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx);


public:
    virtual void LateUpdate() override;
    virtual void UpdateData() override;
    virtual void Render() override;

public:
    TileMap();
    ~TileMap();
};

