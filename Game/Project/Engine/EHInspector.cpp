#include "pch.h"
#include "EHInspector.h"

#include "EHComponentUI.h"

#include "EHTransformUI.h"
#include "EHLight2DUI.h"
#include "EHAnimator2DUI.h"
#include "EHRendererUI.h"
#include "EHRigidbodyUI.h"
#include "EHCameraUI.h"

#include "EHColliderUI.h"

#include "EHGameObject.h"

Inspector::Inspector()
	:UI("Inspector", "##Insepctor")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::RENDERER] = new RendererUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::RENDERER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new ColliderUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY] = new RigidbodyUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	for (UINT _idx = 0;_idx < (UINT)COMPONENT_TYPE::END;_idx++)
	{
		if (nullptr != m_arrComUI[_idx])
		{
			m_arrComUI[_idx]->SetCollapsing(TRUE);
		}
	}

	m_LayerByName.insert(make_pair(LAYER_TYPE::CAMERA, "Camera"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::LIGHT2D, "Light"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::BACKGROUND, "BackGround"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::PLAYER, "Player"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::MONSTER, "Monster"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::TRIGGER, "Trigger"));
	m_LayerByName.insert(make_pair(LAYER_TYPE::TILE, "Tile"));

	m_NameByLayer.insert(make_pair("Camera", LAYER_TYPE::CAMERA));
	m_NameByLayer.insert(make_pair("Light", LAYER_TYPE::LIGHT2D));
	m_NameByLayer.insert(make_pair("BackGround", LAYER_TYPE::BACKGROUND));
	m_NameByLayer.insert(make_pair("Player", LAYER_TYPE::PLAYER));
	m_NameByLayer.insert(make_pair("Monster", LAYER_TYPE::MONSTER));
	m_NameByLayer.insert(make_pair("Trigger", LAYER_TYPE::TRIGGER));
	m_NameByLayer.insert(make_pair("Tile", LAYER_TYPE::TILE));

	m_LayerSelect.insert(make_pair("Camera", FALSE));
	m_LayerSelect.insert(make_pair("Light", FALSE));
	m_LayerSelect.insert(make_pair("BackGround", FALSE));
	m_LayerSelect.insert(make_pair("Player", FALSE));
	m_LayerSelect.insert(make_pair("Monster", FALSE));
	m_LayerSelect.insert(make_pair("Trigger", FALSE));
	m_LayerSelect.insert(make_pair("Tile", FALSE));

	m_LayerName.push_back("Camera");
	m_LayerName.push_back("Light");
	m_LayerName.push_back("BackGround");
	m_LayerName.push_back("Player");
	m_LayerName.push_back("Monster");
	m_LayerName.push_back("Trigger");
	m_LayerName.push_back("Tile");
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

	ShowName();
	ShowLayer();
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

void Inspector::ShowName()
{
	string _strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());

	string _label = "##" + _strName;
	
	ImGui::InputText(_label.c_str(), &_strName);

	m_TargetObject->SetName(wstring(_strName.begin(),_strName.end()));
}

void Inspector::ShowLayer()
{
	map<string, bool>::iterator iter = m_LayerSelect.begin();

	for (;iter != m_LayerSelect.end();iter++)
	{
		iter->second = FALSE;
	}

	LAYER_TYPE _type = m_TargetObject->GetLayerType();

	map<LAYER_TYPE, string>::iterator iter2 = m_LayerByName.find(_type);

	string _layerName = "Non_Select";

	if (iter2 != m_LayerByName.end())
	{
		_layerName = iter2->second;
	}

	iter = m_LayerSelect.find(_layerName);
	iter->second = TRUE;

	ImGui::Text("Layer");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Layer", _layerName.c_str()))
	{
		for (size_t i = 0; i < m_LayerName.size(); i++)
		{
			bool is_selected = (_layerName == m_LayerName[i]);
			iter = m_LayerSelect.find(m_LayerName[i]);
			if (ImGui::Selectable(m_LayerName[i].c_str(), iter->second))
				_layerName = m_LayerName[i].c_str();
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	map<string, LAYER_TYPE>::iterator iter3 = m_NameByLayer.find(_layerName);

	m_TargetObject->SetLayerType(iter3->second);
}