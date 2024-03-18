#include "pch.h"
#include "EHTitleScript.h"

#include <EHLevelMgr.h>
#include <EHKeyMgr.h>
#include "EHGameObject.h"
#include "EHTimeMgr.h"
#include "EHTaskMgr.h"

#include <EHButton.h>

#include "EHGUI_LineScript.h"

TitleScript::TitleScript()
	:m_iCurButton(0)
	, m_bOpen(FALSE)
	, m_PointLight1(nullptr)
	, m_PointLight2(nullptr)
	, m_StartLine(nullptr)
	, m_RightDoor(nullptr)
	, m_LeftDoor(nullptr)
	, m_GameLogo(nullptr)
	, m_pButtons{}
	, m_TitleBG1(nullptr)
	, m_TitleBG2(nullptr)
	, m_pSelectIcon1(nullptr)
	, m_pSelectIcon2(nullptr)
	, m_fSpeed(600.f)
	, m_fOpenTime(0.75f)
	, m_fShake(10.f)
	, m_fSpeed2(200.f)
	, m_bFirst(FALSE)
	, m_bSecond(FALSE)
	, m_bThird(FALSE)
	, m_fAcctime(0.f)
	, m_fAcctime2(0.f)
	, m_fAcctime3(0.f)
	, m_fFrequency(50.f)
	, m_fAmplitude(3.f)
	, m_bStart(TRUE)
	, m_Radius1(0.f)
	, m_Radius2(0.f)
{
	SetName(L"TitleScript");
}

TitleScript::~TitleScript()
{
}

void TitleScript::Start()
{
	if (nullptr != m_PointLight1)
		return;

	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	m_PointLight1 = _curLevel->FindObjectByName(L"PointLight1");
	m_PointLight2 = _curLevel->FindObjectByName(L"PointLight2");
	m_StartLine = _curLevel->FindObjectByName(L"StartLine");

	m_RightDoor = _curLevel->FindObjectByName(L"RightDoor");
	m_LeftDoor = _curLevel->FindObjectByName(L"LeftDoor");
	m_GameLogo = _curLevel->FindObjectByName(L"MainLogo");

	m_TitleBG1 = _curLevel->FindObjectByName(L"TitleBG1");
	m_TitleBG2 = _curLevel->FindObjectByName(L"TitleBG2");

	m_pButtons.push_back(_curLevel->FindObjectByName(L"Start Button"));
	m_pButtons.push_back(_curLevel->FindObjectByName(L"Option Button"));
	m_pButtons.push_back(_curLevel->FindObjectByName(L"Exit Button"));

	m_pSelectIcon1 = _curLevel->FindObjectByName(L"GameSelectIcon1");
	m_pSelectIcon2 = _curLevel->FindObjectByName(L"GameSelectIcon2");

	assert(m_PointLight1);
	assert(m_PointLight2);
	assert(m_StartLine);

	assert(m_RightDoor);
	assert(m_LeftDoor);
	assert(m_GameLogo);

	for (size_t i = 0;i < 3;i++)
	{
		assert(m_pButtons[i]);
	}

	assert(m_pSelectIcon1);
	assert(m_pSelectIcon2);

	m_pButtons[0]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(255.f, 255.f, 255.f, 0.f));
	m_pButtons[1]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(255.f, 255.f, 255.f, 0.f));
	m_pButtons[2]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(255.f, 255.f, 255.f, 0.f));

	m_TitleBG1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"TitleBG1Animation");
	m_TitleBG2->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"TitleBG2Animation");

	m_GameLogo->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));
	m_TitleBG1->Enabled(FALSE);
	m_TitleBG2->Enabled(FALSE);

	m_Radius1 = m_PointLight1->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetRadius();
	m_Radius2 = m_PointLight2->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetRadius();

	m_pSelectIcon1->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));
	m_pSelectIcon2->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));

	Object::FadeIn(m_StartLine, 1.f);

	Object::FadeIn(m_RightDoor, 1.f);
	Object::FadeIn(m_LeftDoor, 1.f);

	Object::FadeInLightRadius(m_PointLight1, m_Radius1, 1.f);
	Object::FadeInLightRadius(m_PointLight2, m_Radius2, 1.f);

	Vec4 _vSelectPos = m_pButtons[m_iCurButton]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	Vec4 _vSelectIcon1 = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Vec4 _vSelectIcon2 = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	_vSelectIcon1.x = -100.f;
	_vSelectIcon1.y = _vSelectPos.y;
	m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vSelectIcon1);

	_vSelectIcon2.x = 100.f;
	_vSelectIcon2.y = _vSelectPos.y;
	m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vSelectIcon2);
}

void TitleScript::Update()
{
	if (m_bStart)
	{
		m_fAcctime3 += DT;
	}

	if (m_fAcctime3 >= 3.f)
	{
		m_bStart = FALSE;
		Object::FadeIn(m_GameLogo, 1.2f);
		m_fAcctime3 = 0.f;

		m_TitleBG1->Enabled(TRUE);
		m_TitleBG2->Enabled(TRUE);
	}

	Transform* _rightDoorTr = m_RightDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _leftDoorTr = m_LeftDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _rightPos = _rightDoorTr->GetRelativePosition();
	Vec4 _leftPos = _leftDoorTr->GetRelativePosition();

	Transform* _logoTr = m_GameLogo->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _logoPos = _logoTr->GetRelativePosition();

	if (m_bOpen)
	{
		// 1.첫 Open
		if (m_bFirst)
		{
			if (_rightPos.x <= 599.f)
			{
				_rightPos.x += m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_rightPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_rightPos.x = 600.f;
				_rightPos.y = 0.f;
				m_fAcctime2 = 0.f;
			}

			if (_leftPos.x >= -599.f)
			{
				_leftPos.x -= m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_leftPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_leftPos.x = -600.f;
				_leftPos.y = 0.f;
				m_bSecond = TRUE;
				m_fAcctime2 = 0.f;
			}
		}

		if (m_bSecond)
		{
			m_fAcctime += DT;
		}

		if (m_bSecond && m_fAcctime >= m_fOpenTime)
		{
			m_bFirst = FALSE;
			m_bSecond = FALSE;
			m_bThird = TRUE;
			m_fAcctime = 0.f;

			Object::FadeInText(m_pButtons[0], 1.f);
			Object::FadeInText(m_pButtons[1], 1.f);
			Object::FadeInText(m_pButtons[2], 1.f);

			Object::FadeIn(m_pSelectIcon1, 1.f);
			Object::FadeIn(m_pSelectIcon2, 1.f);
		}

		if (m_bThird)
		{
			// 2. 두번째 Open
			if (_rightPos.x <= 999.f)
			{
				_rightPos.x += m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_rightPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				// Logo 이동
				_logoPos.z = 10.f;

				_rightPos.x = 1000.f;
				_rightPos.y = 0.f;
				m_fAcctime2 = 0.f;
			}

			if (_leftPos.x >= -999.f)
			{
				_leftPos.x -= m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_leftPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_leftPos.x = -1000.f;
				_leftPos.y = 0.f;
				m_fAcctime2 = 0.f;
				m_bThird = FALSE;
			}
		}
	}
	else
	{
		if (m_bFirst)
		{
			// 1. 첫 Close
			if (_rightPos.x >= 601.f)
			{
				_rightPos.x -= m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_rightPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_rightPos.x = 600.f;
				_rightPos.y = 0.f;
				m_fAcctime2 = 0.f;
			}

			if (_leftPos.x <= -601.f)
			{
				_leftPos.x += m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_leftPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_leftPos.x = -600.f;
				_leftPos.y = 0.f;
				m_fAcctime2 = 0.f;
				m_bSecond = TRUE;

				Object::FadeInLightRadius(m_PointLight1, m_Radius1, 1.f);
				Object::FadeInLightRadius(m_PointLight2, m_Radius2, 1.f);
			}
		}

		if (m_bSecond)
		{
			m_fAcctime += DT;
		}

		if (m_bSecond && m_fAcctime >= m_fOpenTime)
		{
			m_bFirst = FALSE;
			m_bSecond = FALSE;
			m_bThird = TRUE;
			m_fAcctime = 0.f;

			Object::FadeOutText(m_pButtons[0], 0.8f);
			Object::FadeOutText(m_pButtons[1], 0.8f);
			Object::FadeOutText(m_pButtons[2], 0.8f);

			Object::FadeOut(m_pSelectIcon1, 0.8f);
			Object::FadeOut(m_pSelectIcon2, 0.8f);
		}

		// 2. 두번째 Close
		if (m_bThird)
		{
			if (_rightPos.x >= 401.f)
			{
				_rightPos.x -= m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_rightPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_rightPos.x = 400.f;
				_rightPos.y = 0.f;
				m_fAcctime2 = 0.f;
			}

			if (_leftPos.x <= -401.f)
			{
				_leftPos.x += m_fSpeed * DT;
				m_fAcctime2 += m_fFrequency * DT;
				_leftPos.y = m_fAmplitude * sinf(m_fAcctime2);
			}
			else
			{
				_leftPos.x = -400.f;
				_leftPos.y = 0.f;
				m_bThird = FALSE;
				m_fAcctime2 = 0.f;

				m_StartLine->Enabled(TRUE);
			}
		}
	}

	_logoTr->SetRelativePosition(_logoPos);

	_rightDoorTr->SetRelativePosition(_rightPos);
	_leftDoorTr->SetRelativePosition(_leftPos);

	SwitchMove();
}

void TitleScript::LateUpdate()
{
	Transform* _logoTr = m_GameLogo->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _logoPos = _logoTr->GetRelativePosition();

	if (KEY_TAP(KEY::K))
	{
		// 열려 있을 때
		if (m_bOpen)
		{
			// 문 닫기
			m_bOpen = FALSE;
			m_bFirst = TRUE;

			// Logo 이동
			_logoPos.z = -5.f;
		}
		// 문이 닫혀있을 때
		else
		{
			// 문 열기
			m_bOpen = TRUE;
			m_bFirst = TRUE;

			// NoActive start line
			m_StartLine->Enabled(FALSE);

			Object::FadeOutLightRadius(m_PointLight1, m_Radius1, 1.f);
			Object::FadeOutLightRadius(m_PointLight2, m_Radius2, 1.f);
		}
	}
	_logoTr->SetRelativePosition(_logoPos);
}

Vec4 TitleScript::DoorMove(Vec4 _src, Vec4 _dest, float _speed, bool _flag)
{
	if (_flag)
	{
		if (_src.x <= _dest.x - 0.2f)
		{
			_src.x += _speed * DT;
		}
		else
		{
			_src.x = _dest.x;
			_src.y = 0.f;
		}
	}
	else
	{
		if (_src.x >= _dest.x + 0.2f)
		{
			_src.x -= _speed * DT;
		}
		else
		{
			_src.x = -1000.f;
			_src.y = 0.f;
		}
	}

	return _src;
}

void TitleScript::SwitchMove()
{
	if (!m_bOpen)
		return;
	// 버튼 이동
	if (KEY_TAP(KEY::W))
	{
		if (0 < m_iCurButton)
		{
			m_iCurButton--;

			Vec4 _vSelectPos = m_pButtons[m_iCurButton]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			Vec4 _vSelectIcon1 = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			Vec4 _vSelectIcon2 = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			_vSelectIcon1.x = -100.f;
			_vSelectIcon1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon1.x - 50.f, _vSelectIcon1.y, _vSelectIcon1.z, _vSelectIcon1.w),
				_vSelectIcon1, 500.f, GUI_STYLE::MOVERIGHT);

			_vSelectIcon2.x = 100.f;
			_vSelectIcon2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon2.x + 50.f, _vSelectIcon2.y, _vSelectIcon2.z, _vSelectIcon2.w),
				_vSelectIcon2, 500.f, GUI_STYLE::MOVELEFT);
		}
	}

	if (KEY_TAP(KEY::S))
	{
		if (2 > m_iCurButton)
		{
			m_iCurButton++;

			Vec4 _vSelectPos = m_pButtons[m_iCurButton]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			Vec4 _vSelectIcon1 = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			Vec4 _vSelectIcon2 = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			_vSelectIcon1.x = -100.f;
			_vSelectIcon1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon1.x - 50.f, _vSelectIcon1.y, _vSelectIcon1.z, _vSelectIcon1.w),
				_vSelectIcon1, 500.f, GUI_STYLE::MOVERIGHT);

			_vSelectIcon2.x = 100.f;
			_vSelectIcon2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon2.x + 50.f, _vSelectIcon2.y, _vSelectIcon2.z, _vSelectIcon2.w),
				_vSelectIcon2, 500.f, GUI_STYLE::MOVELEFT);
		}
	}

	// 해당 키가 선택될 경우
	if (KEY_PRESSED(J))
	{
		if (0 == m_iCurButton)
		{
			LevelMgr::GetInst()->SelectLevel(L"SlotScene");
		}
		else if (1 == m_iCurButton)
		{

		}
		else
		{
			PostQuitMessage(0);
		}

	}
}

void TitleScript::StartGame()
{
	// Load StartScene
	LevelMgr::GetInst()->SelectLevel(L"SlotScene");
}

void TitleScript::LoadGame()
{
	// Load LoadScene
	// LevelMgr::GetInst()->SelectLevel();
}

void TitleScript::Option()
{
	// Option Screen On
}

void TitleScript::Exit()
{
	// Window Exit

}