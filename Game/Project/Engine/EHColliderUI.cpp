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

	Vec3 _scale = Vec3(0.f);
	Vec3 _pos = Vec3(0.f);
	float _radius = 0.f;

	float _value[3] = {};

	if (COLLIDER_TYPE::BOXCOLLIDER2D == _collider->GetColliderType())
	{
		_scale = _collider->GetOffsetScale();
		_pos = _collider->GetOffsetPos();

		_value[0] = _scale.x;
		_value[1] = _scale.y;
		_value[2] = _scale.z;

		ImGui::Text("Scale	 "); ImGui::SameLine(); ImGui::DragFloat3("##Scale", _value, 0.3f);

		_scale.x = _value[0];
		_scale.y = _value[1];
		_scale.z = _value[2];

		_collider->SetoffSetScale(_scale);
	}
	else if(COLLIDER_TYPE::CIRCLECOLLDIER2D == _collider->GetColliderType())
	{
		CircleCollider2D* _circleCol = dynamic_cast<CircleCollider2D*>(_collider);

		if (nullptr == _circleCol)
			return;

		_radius = _circleCol->GetRadius();
		_pos = _collider->GetOffsetPos();

		ImGui::Text("Radius	 "); ImGui::SameLine(); ImGui::DragFloat("##Scale", &_radius, 0.3f);

		_circleCol->SetRadius(_radius);
	}

	_value[0] = _pos.x;
	_value[1] = _pos.y;
	_value[2] = _pos.z;

	ImGui::Text("Position"); ImGui::SameLine(); ImGui::DragFloat3("##Position", _value , 0.3f);

	_pos.x = _value[0];
	_pos.y = _value[1];
	_pos.z = _value[2];

	_collider->SetOffsetPos(_pos);
}
