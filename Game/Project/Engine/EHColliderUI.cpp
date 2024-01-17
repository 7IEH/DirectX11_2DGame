#include "pch.h"
#include "EHColliderUI.h"

#include "EHGameObject.h"
#include "EHCollider2D.h"

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
}
