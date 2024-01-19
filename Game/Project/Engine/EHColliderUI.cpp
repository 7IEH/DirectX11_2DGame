#include "pch.h"
#include "EHColliderUI.h"

#include "EHGameObject.h"
#include "EHCollider2D.h"

ColliderUI::ColliderUI()
	:ComponentUI("Collider", "##Collider", COMPONENT_TYPE::COLLIDER2D)
{
}

ColliderUI::ColliderUI(const string& _label, const string& _strID, COLLIDER_TYPE _type)
	:ComponentUI(_label,_strID,COMPONENT_TYPE::COLLIDER2D)
	,m_Type(_type)
{
}

ColliderUI::~ColliderUI()
{
}

void ColliderUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Collider2D* _collider = GetTargetObject()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D);

	Vec3 _scale = _collider->GetOffsetScale();
	Vec3 _pos = _collider->GetOffsetPos();

	float _value[3] = { _pos.x,_pos.y,_pos.z };
	ImGui::Text("Position"); ImGui::SameLine(); ImGui::DragFloat3("##Position", _value , 0.3f);
	float _value3[3] = { _scale.x,_scale.y,_scale.z };
	ImGui::Text("Scale	 "); ImGui::SameLine(); ImGui::DragFloat3("##Scale", _value3 , 0.3f);

	_pos.x = _value[0];
	_pos.y = _value[1];
	_pos.z = _value[2];

	_scale.x = _value3[0];
	_scale.y = _value3[1];
	_scale.z = _value3[2];

	_collider->SetOffsetPos(_pos);
	_collider->SetoffSetScale(_scale);
}
