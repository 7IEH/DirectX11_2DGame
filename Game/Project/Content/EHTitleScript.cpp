#include "pch.h"
#include "EHTitleScript.h"

#include <EHLevelMgr.h>
#include <EHKeyMgr.h>
#include "EHGameObject.h"
#include "EHTimeMgr.h"
#include "EHTaskMgr.h"

TitleScript::TitleScript()
	: m_bOpen(FALSE)
	, m_PointLight1(nullptr)
	, m_PointLight2(nullptr)
	, m_StartLine(nullptr)
	, m_RightDoor(nullptr)
	, m_LeftDoor(nullptr)
	, m_GameLogo(nullptr)
	, m_StartButton(nullptr)
	, m_OptionButton(nullptr)
	, m_ExitButton(nullptr)
	, m_TitleBG1(nullptr)
	, m_TitleBG2(nullptr)
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
	, m_fAmplitude(1.5f)
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
	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	m_PointLight1 = _curLevel->FindObjectByName(L"PointLight1");
	m_PointLight2 = _curLevel->FindObjectByName(L"PointLight2");
	m_StartLine = _curLevel->FindObjectByName(L"StartLine");

	m_RightDoor = _curLevel->FindObjectByName(L"RightDoor");
	m_LeftDoor = _curLevel->FindObjectByName(L"LeftDoor");
	m_GameLogo = _curLevel->FindObjectByName(L"MainLogo");

	m_TitleBG1 = _curLevel->FindObjectByName(L"TitleBG1");
	m_TitleBG2 = _curLevel->FindObjectByName(L"TitleBG2");

	//m_StartButton  = _curLevel->FindObjectByName(L"StartButton");
	//m_OptionButton = _curLevel->FindObjectByName(L"OptionButton");
	//m_ExitButton = _curLevel->FindObjectByName(L"ExitButton");

	assert(m_PointLight1);
	assert(m_PointLight2);
	assert(m_StartLine);

	assert(m_RightDoor);
	assert(m_LeftDoor);
	assert(m_GameLogo);

	//assert(m_StartButton);
	//assert(m_OptionButton);
	//assert(m_ExitButton);

	m_TitleBG1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"TitleBG1Animation");
	m_TitleBG2->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"TitleBG2Animation");

	m_GameLogo->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.f));

	m_Radius1 = m_PointLight1->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetRadius();
	m_Radius2 = m_PointLight2->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->GetRadius();

	Object::FadeIn(m_StartLine, 1.f);

	Object::FadeIn(m_RightDoor, 1.f);
	Object::FadeIn(m_LeftDoor, 1.f);

	Object::FadeIn(m_TitleBG1, 1.f);
	Object::FadeIn(m_TitleBG2, 1.f);

	Object::FadeInLightRadius(m_PointLight1, m_Radius1, 1.f);
	Object::FadeInLightRadius(m_PointLight2, m_Radius2, 1.f);
}

void TitleScript::Update()
{
	if (m_bStart)
	{
		m_fAcctime3 += DT;
	}

	if (m_fAcctime3 >= 2.f)
	{
		m_bStart = FALSE;
		Object::FadeIn(m_GameLogo, 1.2f);
		m_fAcctime3 = 0.f;
	}

	Transform* _rightDoorTr = m_RightDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _leftDoorTr = m_LeftDoor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _rightPos = _rightDoorTr->GetRelativePosition();
	Vec4 _leftPos = _leftDoorTr->GetRelativePosition();

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
				m_PointLight1->Enabled(TRUE);
				m_PointLight2->Enabled(TRUE);
			}
		}
	}

	_rightDoorTr->SetRelativePosition(_rightPos);
	_leftDoorTr->SetRelativePosition(_leftPos);
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

			Object::FadeInLightRadius(m_PointLight1, m_Radius1, 1.f);
			Object::FadeInLightRadius(m_PointLight2, m_Radius2, 1.f);
		}
		// 문이 닫혀있을 때
		else
		{
			// 문 열기
			m_bOpen = TRUE;
			m_bFirst = TRUE;

			// Logo 이동
			_logoPos.z = 10.f;

			// NoActive light
			m_PointLight1->Enabled(FALSE);
			m_PointLight2->Enabled(FALSE);

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

void TitleScript::StartGame()
{
	// Load StartScene
	// LevelMgr::GetInst()->SelectLevel();
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