#include "pch.h"
#include "EHInspector.h"

#include "EHComponentUI.h"

#include "EHTransformUI.h"
#include "EHLight2DUI.h"

#include "EHGameObject.h"

Inspector::Inspector()
	:UI("Inspector","##Insepctor")
	,m_TargetObject(nullptr)
	,m_arrComUI{}
{
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);


	for (UINT _idx = 0;_idx < (UINT)COMPONENT_TYPE::END;_idx++)
	{
		if (nullptr != m_arrComUI[_idx])
		{
			m_arrComUI[_idx]->SetCollapsing(TRUE);
		}
	}
}

Inspector::~Inspector()
{
}

void Inspector::Update()
{
}

void Inspector::Render_Update()
{
	if (nullptr == m_TargetObject)
		return;

	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text(strName.c_str());
}

void Inspector::SetTargetObject(GameObject* _obj)
{
	m_TargetObject = _obj;
	
	for (UINT _idx = 0;_idx < (UINT)COMPONENT_TYPE::END;_idx++)
	{
		if (nullptr != m_arrComUI[_idx])
		{
			m_arrComUI[_idx]->SetTargetObject(_obj);
		}
	}
}

void Inspector::SetTargetAsset(Ptr<Asset> _asset)
{
	m_TargetAsset = _asset;
}