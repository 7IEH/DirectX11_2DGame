#include "pch.h"
#include "EHSlotScript.h"

#include <EHLevelMgr.h>
#include <EHKeyMgr.h>

#include "EHRecordManager.h"
#include "EHGUI_LineScript.h"

SlotScript::SlotScript()
	: m_bTrans(FALSE)
	, m_pUnderLine1(nullptr)
	, m_pUnderLine2(nullptr)
	, m_pSelectIcon1(nullptr)
	, m_pSelectIcon2(nullptr)
	, m_Slots{}
	, m_iCurSlot(0)
{
	SetName(L"SlotScript");
}

SlotScript::~SlotScript()
{
}

void SlotScript::Start()
{
	FIND_OBJECT(L"MainLight")->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));

	if (nullptr != m_pUnderLine1)
		return;

	Level* _pCurlevel = LevelMgr::GetInst()->GetCurLevel();

	m_pUnderLine1 = _pCurlevel->FindObjectByName(L"GameSlotUnderLine1");
	m_pUnderLine2 = _pCurlevel->FindObjectByName(L"GameSlotUnderLine2");

	m_pSelectIcon1 = _pCurlevel->FindObjectByName(L"GameSelectIcon1");
	m_pSelectIcon2 = _pCurlevel->FindObjectByName(L"GameSelectIcon2");

	GameObject* _pSlot = _pCurlevel->FindObjectByName(L"GameSlot1");
	GameObject* _pSlotPanel = _pCurlevel->FindObjectByName(L"GameSlotPanel1");
	m_Slots.push_back(std::make_pair(_pSlot, _pSlotPanel));

	_pSlot = _pCurlevel->FindObjectByName(L"GameSlot2");
	_pSlotPanel = _pCurlevel->FindObjectByName(L"GameSlotPanel2");
	m_Slots.push_back(std::make_pair(_pSlot, _pSlotPanel));

	_pSlot = _pCurlevel->FindObjectByName(L"GameSlot3");
	_pSlotPanel = _pCurlevel->FindObjectByName(L"GameSlotPanel3");
	m_Slots.push_back(std::make_pair(_pSlot, _pSlotPanel));

	_pSlot = _pCurlevel->FindObjectByName(L"GameSlot4");
	_pSlotPanel = _pCurlevel->FindObjectByName(L"GameSlotPanel4");
	m_Slots.push_back(std::make_pair(_pSlot, _pSlotPanel));

	_pSlot = _pCurlevel->FindObjectByName(L"GameSlot5");
	_pSlotPanel = _pCurlevel->FindObjectByName(L"GameSlotPanel5");
	m_Slots.push_back(std::make_pair(_pSlot, _pSlotPanel));

	m_pMenuText1 = _pCurlevel->FindObjectByName(L"GUI_Slot_Menu_Text1");
	m_pMenuText2 = _pCurlevel->FindObjectByName(L"GUI_Slot_Menu_Text2");

	for (int i = 1;i <= 5;i++)
	{
		m_vSlotText1.push_back(_pCurlevel->FindObjectByName(L"GUI_Slot_Button_Text"+std::to_wstring(i)));
		m_vSlotText1[i - 1]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(236, 221.f, 192.f, 255.f));
		m_vPos.push_back(m_vSlotText1[i-1]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition());
	}

	for (int i = 1;i <= 5;i++)
	{
		m_vSlotText2.push_back(_pCurlevel->FindObjectByName(L"GUI_Slot_Button_Text_Down" + std::to_wstring(i)));
		m_vSlotText2[i - 1]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(160.f, 149.f, 127.f, 255.f));
	}

	for (int i = 1;i <= 5;i++)
	{
		m_vSlotText3.push_back(_pCurlevel->FindObjectByName(L"GUI_Slot_Number_Text" + std::to_wstring(i)));
		m_vSlotText3[i - 1]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(227.f, 218.f, 189.f, 255.f));
	}

	assert(m_pUnderLine1);
	assert(m_pUnderLine2);

	assert(m_pSelectIcon1);
	assert(m_pSelectIcon2);

	assert(m_pMenuText1);
	assert(m_pMenuText2);

	for (size_t i = 0;i < m_Slots.size();i++)
	{
		assert(m_Slots[i].first && m_Slots[i].second);
	}

	for (auto _pText : m_vSlotText1)
	{
		assert(_pText);
	}

	for (auto _pText : m_vSlotText2)
	{
		assert(_pText);
	}

	for (auto _pText : m_vSlotText3)
	{
		assert(_pText);
	}

	m_pMenuText1->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(236.f, 221.f, 192.f, 255.f));
	m_pMenuText2->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetColor(Vec4(151.f, 141.f, 120.f, 255.f));

	m_pSelectIcon1->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));

	m_pSelectIcon2->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));

	for (size_t i = 0;i < m_Slots.size();i++)
	{
		if (m_iCurSlot == i)
			continue;

		m_Slots[i].first->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			GetMaterial()->SetMaterialParam(COLOR, Vec4(121.f / 255.f, 112.f / 255.f, 93.f / 255.f, 1.f));
		m_Slots[i].second->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			GetMaterial()->SetMaterialParam(COLOR, Vec4(90.f / 255.f, 83.f / 255.f, 67.f / 255.f, 0.6f));
	}

	Vec4 _vSelectPos = m_Slots[m_iCurSlot].first->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	Vec4 _vUnderLine1Pos = m_pUnderLine1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Vec4 _vUnderLine2Pos = m_pUnderLine2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Vec4 _vSelectIcon1Pos = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	Vec4 _vSelectIcon2Pos = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	_vUnderLine1Pos.y = _vSelectPos.y + 51.f;
	m_pUnderLine1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->
		SetRelativePosition(_vUnderLine1Pos);
	_vUnderLine2Pos.y = _vSelectPos.y - 51.f;
	m_pUnderLine2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->
		SetRelativePosition(_vUnderLine2Pos);

	_vSelectIcon1Pos.y = _vSelectPos.y;
	m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->
		SetRelativePosition(_vSelectIcon1Pos);
	_vSelectIcon2Pos.y = _vSelectPos.y;
	m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->
		SetRelativePosition(_vSelectIcon2Pos);

	m_vUnderLinePos1 = _vUnderLine1Pos;
	m_vUnderLinePos2 = _vUnderLine2Pos;

	m_vSelectIconPos1 = _vSelectIcon1Pos;
	m_vSelectIconPos2 = _vSelectIcon2Pos;

	m_vUnderLineScale1 = m_pUnderLine1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();
	m_vUnderLineScale2 = m_pUnderLine2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale();

	m_Slots[m_iCurSlot].first->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));
	m_Slots[m_iCurSlot].second->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(121.f / 255.f, 112.f / 255.f, 93.f / 255.f, 1.f));
}

void SlotScript::Update()
{
	vector<PlayerPref*> _pRecord = RecordManager::GetInst()->GetPlayerPref();

	int _idx = 0;
	for (auto _pPlayerPref : _pRecord)
	{
		if (_pPlayerPref != nullptr)
		{
			wstring _wSaveDay = L"";
			m_vSlotText1[_idx]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"저장한 게임");
			_wSaveDay = std::to_wstring(static_cast<int>(_pPlayerPref->_vSaveDay.x)) + L" / " +
				std::to_wstring(static_cast<int>(_pPlayerPref->_vSaveDay.y)) + L" / " +
				std::to_wstring(static_cast<int>(_pPlayerPref->_vSaveDay.z));
			m_vSlotText1[_idx]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(327.f, m_vPos[_idx].y + 10.f, m_vPos[_idx].z, m_vPos[_idx].w));
			m_vSlotText2[_idx]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(_wSaveDay);
			m_vSlotText2[_idx]->Enabled(TRUE);
		}
		else
		{
			m_vSlotText1[_idx]->GetComponent<Text>(COMPONENT_TYPE::TEXT)->SetText(L"새 게임");
			m_vSlotText1[_idx]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vPos[_idx]);
			m_vSlotText2[_idx]->Enabled(FALSE);
		}
		_idx++;
	}


	if (KEY_TAP(KEY::W))
	{
		Object::Play2DSound(L"\\resource\\Audio\\gui_selector_movement.wav", TRUE, 0.5f);
		if (0 < m_iCurSlot)
		{
			m_iCurSlot--;
			m_bTrans = TRUE;

			Vec4 _vSelectPos = m_Slots[m_iCurSlot].first->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			m_vSelectIconPos1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vSelectIconPos1.x - 50.f, m_vSelectIconPos1.y, m_vSelectIconPos1.z, m_vSelectIconPos1.w),
				m_vSelectIconPos1, 500.f, GUI_STYLE::MOVERIGHT);

			m_vSelectIconPos2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vSelectIconPos2.x + 50.f, m_vSelectIconPos2.y, m_vSelectIconPos2.z, m_vSelectIconPos2.w),
				m_vSelectIconPos2, 500.f, GUI_STYLE::MOVELEFT);

			/*m_vUnderLinePos1.y = _vSelectPos.y + 51.f;
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos1.x, m_vUnderLinePos1.y + 100.f, m_vUnderLinePos1.z, m_vUnderLinePos1.w),
				m_vUnderLinePos1, 500.f, GUI_STYLE::MOVEDOWN);

			m_vUnderLinePos2.y = _vSelectPos.y - 51.f;
			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos2.x, m_vUnderLinePos2.y - 100.f, m_vUnderLinePos2.z, m_vUnderLinePos2.w),
				m_vUnderLinePos2, 500.f, GUI_STYLE::MOVEUP);*/


			// 2번
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4{ 1.f,m_vUnderLineScale1.y,m_vUnderLineScale1.z,m_vUnderLineScale1.w },
				m_vUnderLineScale1, 4000.f, GUI_STYLE::GROW);

			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4{ 1.f,m_vUnderLineScale2.y,m_vUnderLineScale2.z,m_vUnderLineScale2.w },
				m_vUnderLineScale2, 4000.f, GUI_STYLE::GROW);

			// 3번
			/*m_vUnderLinePos1.y = _vSelectPos.y + 51.f;
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos1.x - 400.f, m_vUnderLinePos1.y, m_vUnderLinePos1.z, m_vUnderLinePos1.w),
				m_vUnderLinePos1, 2000.f, GUI_STYLE::MOVERIGHT);

			m_vUnderLinePos2.y = _vSelectPos.y - 51.f;
			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos2.x + 400.f, m_vUnderLinePos2.y, m_vUnderLinePos2.z, m_vUnderLinePos2.w),
				m_vUnderLinePos2, 2000.f, GUI_STYLE::MOVELEFT);*/
		}
	}

	if (KEY_TAP(KEY::S))
	{
		Object::Play2DSound(L"\\resource\\Audio\\gui_selector_movement.wav", TRUE, 0.5f);
		if (4 > m_iCurSlot)
		{
			m_iCurSlot++;
			m_bTrans = TRUE;

			Vec4 _vSelectPos = m_Slots[m_iCurSlot].first->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			m_vSelectIconPos1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vSelectIconPos1.x - 50.f, m_vSelectIconPos1.y, m_vSelectIconPos1.z, m_vSelectIconPos1.w),
				m_vSelectIconPos1, 500.f, GUI_STYLE::MOVERIGHT);

			m_vSelectIconPos2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vSelectIconPos2.x + 50.f, m_vSelectIconPos2.y, m_vSelectIconPos2.z, m_vSelectIconPos2.w),
				m_vSelectIconPos2, 500.f, GUI_STYLE::MOVELEFT);

			// 1번
			/*m_vUnderLinePos1.y = _vSelectPos.y + 51.f;
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos1.x, m_vUnderLinePos1.y + 100.f, m_vUnderLinePos1.z, m_vUnderLinePos1.w),
				m_vUnderLinePos1, 500.f, GUI_STYLE::MOVEDOWN);

			m_vUnderLinePos2.y = _vSelectPos.y - 51.f;
			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos2.x, m_vUnderLinePos2.y - 100.f, m_vUnderLinePos2.z, m_vUnderLinePos2.w),
				m_vUnderLinePos2, 500.f, GUI_STYLE::MOVEUP);*/

				// 2번
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4{ 1.f,m_vUnderLineScale1.y,m_vUnderLineScale1.z,m_vUnderLineScale1.w },
				m_vUnderLineScale1, 4000.f, GUI_STYLE::GROW);

			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4{ 1.f,m_vUnderLineScale2.y,m_vUnderLineScale2.z,m_vUnderLineScale2.w },
				m_vUnderLineScale2, 4000.f, GUI_STYLE::GROW);

			// 3번
		/*	m_vUnderLinePos1.y = _vSelectPos.y + 51.f;
			_script = m_pUnderLine1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos1.x - 600.f, m_vUnderLinePos1.y, m_vUnderLinePos1.z, m_vUnderLinePos1.w),
				m_vUnderLinePos1, 2000.f, GUI_STYLE::MOVERIGHT);

			m_vUnderLinePos2.y = _vSelectPos.y - 51.f;
			_script = m_pUnderLine2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(m_vUnderLinePos2.x + 600.f, m_vUnderLinePos2.y, m_vUnderLinePos2.z, m_vUnderLinePos2.w),
				m_vUnderLinePos2, 2000.f, GUI_STYLE::MOVELEFT);*/
		}
	}

	if (KEY_TAP(KEY::J))
	{
		// 해당 slot 실행
		// 난이도 추가
		RecordManager::GetInst()->CreateSaveFile(m_iCurSlot);

		PlayerPref* _pref = RecordManager::GetInst()->GetCurrentPlayerPref();

		if (_pref->_bTutorial == 0 && _pref->_ePlace == PLACE::TUTORIAL)
		{
			Object::Stop2DSound(L"\\resource\\Audio\\main_menu_door_opened_loop.wav");
			Object::Stop2DSound(L"\\resource\\Audio\\main_menu_wind.wav");
			SceneManager::SelectScene(L"TutorialScene");
		}
		else
		{
			if (_pref->_ePlace == PLACE::TOWN)
			{
				Object::Stop2DSound(L"\\resource\\Audio\\main_menu_door_opened_loop.wav");
				Object::Stop2DSound(L"\\resource\\Audio\\main_menu_wind.wav");
				SceneManager::SelectScene(L"TownScene");
			}
		}
	}

	if (KEY_TAP(KEY::Z))
	{
		if (_pRecord[m_iCurSlot] != nullptr)
		{
			RecordManager::GetInst()->DeleteSaveFile(m_iCurSlot);
		}
	}

	if (KEY_TAP(KEY::ESC))
	{
		LevelMgr::GetInst()->SelectLevel(L"EnterScene");
	}
}

void SlotScript::LateUpdate()
{
	if (m_bTrans)
	{
		// 해당 변화
		for (size_t i = 0;i < m_Slots.size();i++)
		{
			if (m_iCurSlot == i)
				continue;

			m_Slots[i].first->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
				GetMaterial()->SetMaterialParam(COLOR, Vec4(121.f / 255.f, 112.f / 255.f, 93.f / 255.f, 1.f));
			m_Slots[i].second->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
				GetMaterial()->SetMaterialParam(COLOR, Vec4(90.f / 255.f, 83.f / 255.f, 67.f / 255.f, 0.6f));
		}

		// 2번
		Vec4 _vSlot = m_Slots[m_iCurSlot].first->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

		m_vUnderLinePos1.y = _vSlot.y + 51.f;
		m_vUnderLinePos2.y = _vSlot.y - 51.f;

		m_pUnderLine1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vUnderLinePos1);
		m_pUnderLine2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(m_vUnderLinePos2);

		m_Slots[m_iCurSlot].first->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));
		m_Slots[m_iCurSlot].second->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
			GetMaterial()->SetMaterialParam(COLOR, Vec4(121.f / 255.f, 112.f / 255.f, 93.f / 255.f, 1.f));
	}
}