#include "pch.h"
#include "EHText.h"

#include "EHDevice.h"
#include "EHFontMgr.h"

#include "EHGameObject.h"

Text::Text()
	:Component(COMPONENT_TYPE::TEXT)
	, m_Color(0xffffffff)
	, m_sFont(L"Arial")
	, m_eWeight(DWRITE_FONT_WEIGHT_NORMAL)
	, m_fFontSize(32.f)
{
}

Text::~Text()
{
}

void Text::Update()
{
}

void Text::Render()
{
	Vec4 _position = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	FontMgr::GetInst()->DrawingText(m_sTextInput, m_Color, m_sFont, m_eWeight, _position.x, _position.y, m_fFontSize);
}