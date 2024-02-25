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
    
    bool                m_Velocity;
    bool                m_Scale;
    bool                m_Noise;
    bool                m_Color;

public:
    virtual void Render_Update();

private:
    void MeshRenderer_Update();
    void TileMap_Update();
    void Particle_Update();
    void Canvas_Update();

public:
    RendererUI();
    virtual ~RendererUI();
};

