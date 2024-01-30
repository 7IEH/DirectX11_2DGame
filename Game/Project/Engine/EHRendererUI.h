#pragma once
#include "EHComponentUI.h"
class RendererUI :
    public ComponentUI
{
private:
    bool				m_ColFlag;

    vector<GameObject*>	m_Collider;
    
    string				m_CurTile;
    string				m_prevTile;
    
    // Particle
    string              m_CurSpace;
    string              m_CurSprite;
    string              m_CurNoise;
    string              m_CurShape;

public:
    virtual void Render_Update();

private:
    void MeshRenderer_Update();
    void TileMap_Update();
    void Particle_Update();

public:
    RendererUI();
    virtual ~RendererUI();
};

