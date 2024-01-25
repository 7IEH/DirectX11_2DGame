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

