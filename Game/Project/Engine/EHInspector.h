#pragma once

#include "EHUI.h"

#include "EHGameObject.h"
#include "EHAsset.h"

class ComponentUI;

class Inspector
	:public UI
{
private:
	map<LAYER_TYPE, string>	m_LayerByName;
	map<string, LAYER_TYPE>	m_NameByLayer;

	vector<string>		m_LayerName;

	map<string,bool>	m_LayerSelect;

	GameObject*		m_TargetObject;
	Ptr<Asset>		m_TargetAsset;

	ComponentUI*	m_arrComUI[(UINT)COMPONENT_TYPE::END];
	
public:
	virtual void Update() override;
	virtual void Render_Update() override;

public:
	void SetTargetObject(GameObject* _obj);
	void SetTargetAsset(Ptr<Asset> _asset);


private:
	void ShowName();
	void ShowLayer();

public:
	Inspector();
	virtual ~Inspector();
};

