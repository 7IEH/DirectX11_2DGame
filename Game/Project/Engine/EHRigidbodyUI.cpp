#include "pch.h"
#include "EHRigidbodyUI.h"

RigidbodyUI::RigidbodyUI()
	:ComponentUI("Rigidbody","##Rigidbody",COMPONENT_TYPE::RIGIDBODY)
{
}

RigidbodyUI::~RigidbodyUI()
{
}

void RigidbodyUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;
}