#include "pch.h"
#include "Animator2DUI.h"

#include "EHGameObject.h"

#include "EHAnimator2D.h"
#include "EHAnimation2D.h"

Animator2DUI::Animator2DUI()
	:ComponentUI("Animator2D","##Animator2D",COMPONENT_TYPE::ANIMATOR2D)
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Animator2D* _animator = GetTargetObject()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
}