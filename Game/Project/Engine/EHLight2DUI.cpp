#include "pch.h"
#include "EHLight2DUI.h"

#include "EHGameObject.h"
#include "EHLIght2D.h"

Light2DUI::Light2DUI()
	:ComponentUI("Light2D","##Light2D",COMPONENT_TYPE::LIGHT2D)
{
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
	float _Radius = _light2d->GetRadius();
	float _Angle = _light2d->GetAngle();
	Vec4 _Color = _light2d->GetColor();
	Vec4 _Ambient = _light2d->GetAmbient();

	ImGui::Text("LightSetting");
	ImGui::InputScalar("Radius", ImGuiDataType_Float, &_Radius, 0, 0, "%.f", flags);
	ImGui::InputScalar("Angle", ImGuiDataType_Float, &_Angle, 0, 0, "%.f", flags);

	ImGui::Text("LightColor");
	ImGui::InputScalar("RED", ImGuiDataType_Float, &_Color.x, 0, 0, "%.f", flags);
	ImGui::InputScalar("BLUE", ImGuiDataType_Float, &_Color.y, 0, 0, "%.f", flags);
	ImGui::InputScalar("GREEN", ImGuiDataType_Float, &_Color.z, 0, 0, "%.f", flags);

	ImGui::Text("LightAmbient");
	ImGui::InputScalar("RED_A", ImGuiDataType_Float, &_Ambient.x, 0, 0, "%.f", flags);
	ImGui::InputScalar("BLUE_A", ImGuiDataType_Float, &_Ambient.y, 0, 0, "%.f", flags);
	ImGui::InputScalar("GREEN_A", ImGuiDataType_Float, &_Ambient.z, 0, 0, "%.f", flags);

	ImGui::Text("Type");
	ImGui::InputScalar("RIGHTTYPE", ImGuiDataType_Float, &_typeInfo, 0, 0, "%.f", flags);

	_light2d->SetAmbient(_Ambient);
	_light2d->SetColor(_Color);

	int _push = (int)_typeInfo;
	_light2d->SetLightType(LIGHT_TYPE(_push));
	_light2d->SetRadius(_Radius);
	_light2d->SetAngle(_Angle);
}