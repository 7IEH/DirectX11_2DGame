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
#include "EHScriptUI.h"

#include "EHTextUI.h"
#include "EHButtonUI.h"

#include "EHGameObject.h"
#include "EHAddComponent.h"

#include "EHScriptMgr.h"

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

	m_arrComUI[(UINT)COMPONENT_TYPE::TEXT] = new TextUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TEXT]);

	m_arrComUI[(UINT)COMPONENT_TYPE::BUTTON] = new ButtonUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::BUTTON]);

	map<wstring, Script*> _scripts = ScriptMgr::GetInst()->GetScripts();
	map<wstring, Script*>::iterator iter = _scripts.begin();
	for (;iter != _scripts.end();iter++)
	{
		ScriptUI* _scriptUI = new ScriptUI(EH::ConvertString(iter->first));
		_scriptUI->SetCollapsing(TRUE);
		m_ScriptUI.push_back(_scriptUI);
		AddChildUI(_scriptUI);
	}

	m_AddCompoentUI = new AddComponent;
	AddChildUI(m_AddCompoentUI);

	for (UINT _idx = 0;_idx < (UINT)COMPONENT_TYPE::END;_idx++)
	{
		if (nullptr != m_arrComUI[_idx])
		{
			m_arrComUI[_idx]->SetCollapsing(TRUE);
		}
	}
	
	vector<string> _vLayerName; _vLayerName.clear();
	_vLayerName.push_back("ESSENTAIL_CAMERA");
	_vLayerName.push_back("ESSENTAIL_LIGHT");
	_vLayerName.push_back("OBJECT_BG1");
	_vLayerName.push_back("OBJECT_TEXT1");
	_vLayerName.push_back("OBJECT_BG2");
	_vLayerName.push_back("OBJECT_PLAYER");
	_vLayerName.push_back("OBJECT_MONSTER");
	_vLayerName.push_back("OBJECT_TRIGGER");
	_vLayerName.push_back("OBJECT_STRUCTURE_WALL_RIGHT");
	_vLayerName.push_back("OBJECT_STRUCTURE_WALL_LEFT");
	_vLayerName.push_back("OBJECT_STRUCTURE_WALL_TOP");
	_vLayerName.push_back("OBJECT_STRUCTURE_WALL_BOTTOM");
	_vLayerName.push_back("OBJECT_TILE");
	_vLayerName.push_back("OBJECT_PARTICLE");
	_vLayerName.push_back("OBJECT_UI");
	_vLayerName.push_back("OBJECT_OBSTACLE_HOLE");
	_vLayerName.push_back("OBJECT_OBSTACLE_ROCK");
	_vLayerName.push_back("PLAYER_PROJECTILE");
	_vLayerName.push_back("ENEMY_PROJECTILE");

	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_LayerByName.insert(make_pair(LAYER_TYPE(i), _vLayerName[i]));
		m_NameByLayer.insert(make_pair(_vLayerName[i], LAYER_TYPE(i)));
		m_LayerSelect.insert(make_pair(_vLayerName[i], FALSE));
		m_LayerName.push_back(_vLayerName[i]);
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

	for (size_t _idx = 0;_idx < m_ScriptUI.size();_idx++)
	{
		if (nullptr != m_ScriptUI[_idx])
		{
			m_ScriptUI[_idx]->SetTargetObject(_obj);
		}
	}

	m_AddCompoentUI->SetTargetObject(m_TargetObject);
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

	m_TargetObject->SetName(wstring(_strName.begin(), _strName.end()));
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