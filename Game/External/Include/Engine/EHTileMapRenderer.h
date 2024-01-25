#pragma once

#include "EHComponentUI.h"

class TileMapRenderer
	:public ComponentUI
{
private:
	bool				m_ColFlag;

	vector<GameObject*>	m_Collider;

	string				m_CurTile;
	string				m_prevTile;

public:
	virtual void Render_Update()override;

public:
	TileMapRenderer();
	virtual ~TileMapRenderer();
};

