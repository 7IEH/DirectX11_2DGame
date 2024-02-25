#include "pch.h"
#include "EHButtonUI.h"

ButtonUI::ButtonUI()
	:ComponentUI("ButtonUI", "##ButtonUI",COMPONENT_TYPE::BUTTON)
{
}

ButtonUI::~ButtonUI()
{
}

void ButtonUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;
}