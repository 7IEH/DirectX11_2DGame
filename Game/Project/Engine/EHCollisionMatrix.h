#pragma once

#include "EHUI.h"

class CollisionMatrix
	:public UI
{
private:
	UINT m_CollisionMatrix[(UINT)LAYER_TYPE::END];
	
	vector<string> m_LayerName;

	UINT		   m_MaxLength;
public:
	virtual void Render_Update() override;

public:
	CollisionMatrix();
	virtual ~CollisionMatrix();
};