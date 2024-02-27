#include "pch.h"
#include "EHButton.h"

#include "EHGameObject.h"
#include "EHKeyMgr.h"

#include "EHEngine.h"

Button::Button()
	:Component(COMPONENT_TYPE::BUTTON)
	, m_State(BUTTON_STATE::NONE)
{
}

Button::~Button()
{
}

// 이 부분 나중에 Object Picking으로 바꾸기
void Button::Update()
{
	Vec2 _mousePos = KeyMgr::GetInst()->GetMousePos();

	Transform* _tr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Text* _btn = GetOwner()->GetComponent<Text>(COMPONENT_TYPE::TEXT);

	assert(_tr);
	Vec2 _resolution = Engine::GetInst()->GetResolution();
	Vec4 _originPos = _tr->GetRelativePosition();
	Vec4 _buttonPos = Vec4((_originPos.x * -1.f) + _resolution.x / 2.f, (_originPos.y * -1.f) + _resolution.y / 2.f, 0.f, 0.f);

	Vec4 _buttonScale = _tr->GetRelativeScale();

	// 마우스가 버튼 on
	if ((_buttonPos.x - _buttonScale.x / 2.f <= _mousePos.x && _buttonPos.x + _buttonScale.x / 2.f >= _mousePos.x)
		&& (_buttonPos.y - _buttonScale.y / 2.f <= _mousePos.y && _buttonPos.y + _buttonScale.y / 2.f >= _mousePos.y))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_State = BUTTON_STATE::PRESSED;
		}
		else
		{
			m_State = BUTTON_STATE::SELECTED;
		}
	}
	// 마우스가 버튼 released	
	else
	{
		m_State = BUTTON_STATE::NONE;
	}
}

void Button::Save(string _path)
{
	std::wofstream _file(_path, std::fstream::out | std::fstream::app);

	_file << L"BUTTON\n";
	for (UINT i = 0;i < (UINT)BUTTON_STATE::END;i++)
	{
		if (nullptr != m_Images[i])
		{
			_file << m_Images[i]->GetName() + L'\n';
		}
		else
		{
			_file << L" \n";
		}
	}

	_file.close();
}