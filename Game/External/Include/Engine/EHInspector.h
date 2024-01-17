#pragma once

#include "EHUI.h"

#include "EHGameObject.h"
#include "EHAsset.h"

class ComponentUI;

class Inspector
	:public UI
{
private:
	GameObject*		m_TargetObject;
	Ptr<Asset>		m_TargetAsset;

	ComponentUI*	m_arrComUI[(UINT)COMPONENT_TYPE::END];
	
public:
	virtual void Update() override;
	virtual void Render_Update() override;

public:
	void SetTargetObject(GameObject* _obj);
	void SetTargetAsset(Ptr<Asset> _asset);

public:
	Inspector();
	virtual ~Inspector();
};

