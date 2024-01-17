#pragma once

#include "EHUI.h"

class ComponentUI
	: public UI
{
private:
	GameObject* m_TargetObject;
	COMPONENT_TYPE m_Type;

	string m_ComponentTitle;

public:
	virtual void Render_Update();

public:
	GameObject* GetTargetObject() { return m_TargetObject; }
	void SetTargetObject(GameObject* _target);
	void SetComopnentTitle(const string& _title) { m_ComponentTitle = _title; }
	COMPONENT_TYPE GetType() { return m_Type; }

public:
	ComponentUI(const string& _label, const string& _strID,COMPONENT_TYPE _type);
	virtual ~ComponentUI();
};

