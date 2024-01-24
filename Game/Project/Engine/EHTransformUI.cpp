#include "pch.h"
#include "EHTransformUI.h"

#include "EHGameObject.h"
#include "EHLevelMgr.h"

TransformUI::TransformUI()
	:ComponentUI("Transform", "##Transform", COMPONENT_TYPE::TRANSFORM)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Transform* _tr = GetTargetObject()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	// Script*
	// Light*
	// Camera*

	Vec4 _scale = _tr->GetRelativeScale();
	Vec4 _pos = _tr->GetRelativePosition();
	Vec3 _rot = _tr->GetRelativeRotation();

	float _value[3] = { _pos.x,_pos.y,_pos.z };
	ImGui::Text("Position"); ImGui::SameLine(100.f); ImGui::DragFloat3("##Position", _value);
	float _value2[3] = { _rot.x,_rot.y,_rot.z };
	ImGui::Text("Rotation"); ImGui::SameLine(100.f); ImGui::DragFloat3("##Rotation", _value2);
	float _value3[3] = { _scale.x,_scale.y,_scale.z };
	ImGui::Text("Scale	 "); ImGui::SameLine(100.f); ImGui::DragFloat3("##Scale  ", _value3);

	_pos.x = _value[0];
	_pos.y = _value[1];
	_pos.z = _value[2];

	_rot.x = _value2[0];
	_rot.y = _value2[1];
	_rot.z = _value2[2];

	_scale.x = _value3[0];
	_scale.y = _value3[1];
	_scale.z = _value3[2];

	_tr->SetRelativePosition(_pos);
	_tr->SetRelativeRotation(_rot);
	_tr->SetRelativeScale(_scale);
}
