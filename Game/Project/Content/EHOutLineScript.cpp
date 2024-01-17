#include "pch.h"
#include "EHOutLineScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

OutLineScript::OutLineScript()
	:m_Owner(nullptr)
{
}

OutLineScript::~OutLineScript()
{
}

void OutLineScript::Update()
{
	if (GetOwner() == nullptr)
		return;
	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _ownertr = m_Owner->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _pos = _tr->GetRelativePosition();

	_pos.x = _ownertr->GetRelativePosition().x;
	_pos.y = _ownertr->GetRelativePosition().y;
	
	_tr->SetRelativePosition(_pos);
}