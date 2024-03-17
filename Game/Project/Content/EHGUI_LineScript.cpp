#include "pch.h"
#include "EHGUI_LineScript.h"

#include <EHTimeMgr.h>
#include <EHGameObject.h>

GUI_LineScript::GUI_LineScript()
	: m_eStyle(GUI_STYLE::None)
	, m_bStart(FALSE)
	, m_vCurState{}
	, m_vTargetState{}
	, m_fSpeed(0.f)
{
	SetName(L"GUI_LineScript");
}

GUI_LineScript::~GUI_LineScript()
{
}

void GUI_LineScript::LateUpdate()
{
	switch (m_eStyle)
	{
	case GUI_STYLE::GROW:
		Grow();
		break;
	case GUI_STYLE::MOVERIGHT:
		MoveRight();
		break;
	case GUI_STYLE::MOVELEFT:
		MoveLeft();
		break;
	case GUI_STYLE::MOVEUP:
		MoveUp();
		break;
	case GUI_STYLE::MOVEDOWN:
		MoveDown();
		break;
	case GUI_STYLE::None:
		break;
	default:
		break;
	}
}

void GUI_LineScript::Grow()
{
	if (m_vTargetState.x <= m_vCurState.x)
	{
		m_vTargetState.x = m_vCurState.x;
		m_eStyle = GUI_STYLE::None;
	}
	else
	{
		m_vCurState.x += m_fSpeed * DT;
	}

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativeScale(m_vCurState);
}

void GUI_LineScript::MoveUp()
{
	if (m_vTargetState.y <= m_vCurState.y)
	{
		m_vTargetState.y = m_vCurState.y;
		m_eStyle = GUI_STYLE::None;
	}
	else
	{
		m_vCurState.y += m_fSpeed * DT;
	}

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vCurState);
}

void GUI_LineScript::MoveDown()
{
	if (m_vTargetState.y >= m_vCurState.y)
	{
		m_vTargetState.y = m_vCurState.y;
		m_eStyle = GUI_STYLE::None;
	}
	else
	{
		m_vCurState.y -= m_fSpeed * DT;
	}

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vCurState);
}

void GUI_LineScript::MoveRight()
{
	if (m_vTargetState.x <= m_vCurState.x)
	{
		m_vTargetState.x = m_vCurState.x;
		m_eStyle = GUI_STYLE::None;
	}
	else
	{
		m_vCurState.x += m_fSpeed * DT;
	}

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vCurState);
}

void GUI_LineScript::MoveLeft()
{
	if (m_vTargetState.x >= m_vCurState.x)
	{
		m_vTargetState.x = m_vCurState.x;
		m_eStyle = GUI_STYLE::None;
	}
	else
	{
		m_vCurState.x -= m_fSpeed * DT;
	}

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vCurState);
}

