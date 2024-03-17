#pragma once
#include <EHScript.h>

enum class GUI_STYLE
{
	GROW,
	MOVERIGHT,
	MOVELEFT,
	MOVEUP,
	MOVEDOWN,
	None,
};

class GUI_LineScript :
	public Script
{
private:
	GUI_STYLE m_eStyle;
	bool m_bStart;

	Vec4    m_vCurState;
	Vec4    m_vTargetState;

	float   m_fSpeed;

public:
	void SetEvent(const Vec4& _vCurState, const Vec4& _vTargetState, float _fSpeed, GUI_STYLE _eStyle)
	{
		m_eStyle = _eStyle;
		m_vCurState = _vCurState;
		m_vTargetState = _vTargetState;
		m_fSpeed = _fSpeed;
	}

public:
	virtual void LateUpdate();

private:
	void Grow();
	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

public:
	CLONE(GUI_LineScript);

	GUI_LineScript();
	virtual ~GUI_LineScript();
};

