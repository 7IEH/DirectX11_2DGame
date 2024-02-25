#include "pch.h"
#include "EHLight2DUI.h"

#include "EHGameObject.h"
#include "EHLIght2D.h"

Light2DUI::Light2DUI()
	:ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	m_NameByLayer.insert(make_pair("Directional", LIGHT_TYPE::DIRECTIONAL));
	m_NameByLayer.insert(make_pair("Point", LIGHT_TYPE::POINT));
	m_NameByLayer.insert(make_pair("Spot", LIGHT_TYPE::SPOT));

	m_LayerByName.insert(make_pair(LIGHT_TYPE::DIRECTIONAL, "Directional"));
	m_LayerByName.insert(make_pair(LIGHT_TYPE::POINT, "Point"));
	m_LayerByName.insert(make_pair(LIGHT_TYPE::SPOT, "Spot"));

	m_LayerName.push_back("Directional");
	m_LayerName.push_back("Point");
	m_LayerName.push_back("Spot");

	m_LayerSelect.insert(make_pair("Directional", FALSE));
	m_LayerSelect.insert(make_pair("Point", FALSE));
	m_LayerSelect.insert(make_pair("Spot", FALSE));
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	LIght2D* _light2d = GetTargetObject()->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);

	ImGuiInputTextFlags flags = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;

	LIGHT_TYPE _type = _light2d->GetLightType();
	float _typeInfo = (float)_type;
	Vec4 _Color = _light2d->GetColor();
	Vec4 _Ambient = _light2d->GetAmbient();

	map<LIGHT_TYPE, string>::iterator iter = m_LayerByName.find(_type);

	string _curProejction = iter->second;

	map<string, bool>::iterator iter2;

	ImGui::Text("Light Type");
	ImGui::SameLine(105.f);
	if (ImGui::BeginCombo("##lightType", _curProejction.c_str()))
	{
		for (UINT _idx = 0; _idx < (UINT)LIGHT_TYPE::END;_idx++)
		{
			bool is_selected = (_curProejction == m_LayerName[_idx]);
			iter2 = m_LayerSelect.find(m_LayerName[_idx]);
			if (ImGui::Selectable(m_LayerName[_idx].c_str(), iter2->second))
				_curProejction = m_LayerName[_idx].c_str();
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	map<string, LIGHT_TYPE>::iterator iter3 = m_NameByLayer.find(_curProejction);

	_light2d->SetLightType(iter3->second);

	if (_light2d->GetLightType() == LIGHT_TYPE::POINT)
	{
		float _Radius = _light2d->GetRadius();

		ImGui::Text("LightSetting");
		ImGui::Text("Radius");
		ImGui::SameLine(100.f);
		ImGui::DragFloat("##Radius",&_Radius, 0.3f);

		_light2d->SetRadius(_Radius);
	}
	else if(_light2d->GetLightType() == LIGHT_TYPE::SPOT)
	{
		float _Radius = _light2d->GetRadius();
		float _Angle = _light2d->GetAngle();

		ImGui::Text("LightSetting");
		ImGui::Text("Radius");
		ImGui::SameLine();
		ImGui::DragFloat("##Radius", &_Radius, 0.3f);
		ImGui::Text("Angle");
		ImGui::SameLine();
		ImGui::DragFloat("##Angle",&_Angle, 0.3f);

		_light2d->SetRadius(_Radius);
		_light2d->SetAngle(_Angle);
	}
	
	ImGui::Separator();
	ImGui::Text("Light Color");
	ImGui::SameLine();

	ImVec4 _color = ImVec4(_Color.x, _Color.y, _Color.z, _Color.z);
	ImGui::ColorEdit3("##MyColor##2f", (float*)&_color);
	ImGui::ColorPicker3("##MyColor##6", (float*)&_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	_Color.x = _color.x;
	_Color.y = _color.y;
	_Color.z = _color.z;
	_Color.w = _color.w;


	ImGui::Separator();
	ImGui::Text("Light Ambient");
	ImGui::SameLine();

	_color = ImVec4(_Ambient.x, _Ambient.y, _Ambient.z, _Ambient.z);
	ImGui::ColorEdit3("##MyColor##3f", (float*)&_color);
	ImGui::ColorPicker3("##MyColor##7", (float*)&_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	_Ambient.x = _color.x;
	_Ambient.y = _color.y;
	_Ambient.z = _color.z;
	_Ambient.w = _color.w;

	_light2d->SetAmbient(_Ambient);
	_light2d->SetColor(_Color);	
}