#pragma once

#include "EHUI.h"

#include "EHGameObject.h"
#include "EHAsset.h"
#include "EHAddComponent.h"

class ComponentUI;
class ScriptUI;

class Inspector
	:public UI
{
private:
	map<LAYER_TYPE, string>		m_LayerByName;
	map<string, LAYER_TYPE>		m_NameByLayer;

	vector<string>				m_LayerName;

	map<string,bool>		    m_LayerSelect;

	GameObject*					m_TargetObject;
	Ptr<Asset>					m_TargetAsset;

	ComponentUI*				m_arrComUI[(UINT)COMPONENT_TYPE::END];
	
	vector<ScriptUI*>			m_ScriptUI;

	AddComponent*				m_AddCompoentUI;
public:
	virtual void Update() override;
	virtual void Render_Update() override;

public:
	void SetTargetObject(GameObject* _obj);
	void SetTargetAsset(Ptr<Asset> _asset);

	GameObject* GetTargetObject() { return m_TargetObject; }

private:
	void ShowName();
	void ShowLayer();

public:
	Inspector();
	virtual ~Inspector();
};

