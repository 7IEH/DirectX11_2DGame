#include "pch.h"
#include "EHPlayerScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHKeyMgr.h"
#include <EHLevelMgr.h>

#include "EHTimeMgr.h"

#include "EHImGUIMgr.h"
#include "EHAssetMgr.h"

#include "EHItemScript.h"
#include "EHTriggerScript.h"
#include "EHGUI_LineScript.h"

#include <EHAnimation2D.h>

#include <EHRenderMgr.h>

#include "EHEnemyScript.h"
#include "EHProjecTileScript.h"

#include "EHRoomManager.h"

PlayerScript::PlayerScript()
	: m_eDir(Dir::LEFT)
	, m_eState(State::Idle)
	, m_pPlayerPref(nullptr)
	, m_fAccTime(0.f)
	, m_fClifTime(0.f)
	, m_bFalling(FALSE)
	, m_vPrevPos{}
	, m_fInvincibilityTime(0.f)
	, m_fGliterTime(0.f)
	, m_iGliterFlag(0)
	, m_bWhiteGliter(FALSE)
	, m_bRedGliter(FALSE)
	, m_fTriggerTime(0.f)
	, m_bInvincibility(FALSE)
	, m_bChestOpen(FALSE)
	, m_bInventoryOpen(FALSE)
	, m_vEnterWall_Move(4, FALSE)
	, m_vEnterWall_Dodge(4, FALSE)
	, m_pInventoryInface(nullptr)
	, m_pChestIntreface(nullptr)
	, m_pInventoryInterface2(nullptr)
	, m_pEquipSlotObjs{}
	, m_pInfoText{}
	, m_pUICurSor(nullptr)
	, m_pChestObject(nullptr)
	, m_vChestPos{ 0,0 }
	, m_vInventoryPos{ 0,0 }
	, m_pInventorySlotObjs{}
	, m_pChestSlotObjs{}
	, m_vItemName{}
	, m_vOutInventoryPos{}
	, m_vInInventorypos{}
	, m_vOutChestPos{}
	, m_vInChestPos{}
	, m_bMoved(FALSE)
	, m_fPauseTime(0.f)
	, m_pPlayerCam(nullptr)
	, m_eCameraMove(Dir::None)
	, m_pSpearCollider(nullptr)
	, m_pOneHandCollider(nullptr)
	, m_fComboTime(0.f)
	, m_iComboCount(1)
	, m_iCurComboCount(1)
	, m_pBottomWall(nullptr)
	, m_pTopWall(nullptr)
	, m_pRightWall(nullptr)
	, m_pLeftWall(nullptr)
	, m_fSelectorSize(80.f)
	, m_iCurButton(0)
	, m_fSceneChangeTime(0.f)
	, m_pMainLight(nullptr)
	, m_pObject_Village_Fade_BG(nullptr)
	, m_pSelectIcon1(nullptr)
	, m_pSelectIcon2(nullptr)
	, m_eSceneTrigger(TRIGGER_TYPE::END)
	, m_bSceneChange(TRUE)
	, m_fDungeonEnterTime(0.f)
	, m_pDungeonPortal1(nullptr)
{
	SetName(L"PlayerScript");
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Awake()
{
	m_vOutInventoryPos = Vec4(-348.f, -900.f, 0.f, 0.f);
	m_vInInventorypos = Vec4(-348.f, 1.f, 0.f, 0.f);

	m_vOutChestPos = Vec4(1300.f, 30.f, 0.f, 0.f);
	m_vInChestPos = Vec4(331.f, 30.f, 0.f, 0.f);

	m_pInventoryInface = FIND_OBJECT(L"UI_Player_Chest_Inventory");
	m_pChestIntreface = FIND_OBJECT(L"UI_Player_Chest_Chest");
	m_pUICurSor = FIND_OBJECT(L"UI_Player_Selector");
	m_pPlayerCam = FIND_OBJECT(L"MainCamera");
	m_pObject_Village_Fade_BG = FIND_OBJECT(L"Object_Village_Fade_BG");

	m_pInventoryInterface2 = FIND_OBJECT(L"UI_Player_Inventory");

	m_pDungeonPortal1 = FIND_OBJECT(L"Object_Enterance_Portal1");

	for (int i = 0;i < 20;i++)
	{
		m_pInventorySlotObjs[i] = FIND_OBJECT(L"GUI_Inventoy_Slot" + std::to_wstring(i + 1));
	}

	/*for (int i = 0;i < 28;i++)
	{
		m_pChestSlotObjs[i] = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GUI_Chest_Slot" + std::to_wstring(i + 1));
	}*/

	for (int i = 0;i < 8;i++)
	{
		m_pEquipSlotObjs[i] = FIND_OBJECT(L"GUI_Equip_Slot" + std::to_wstring(i + 1));
	}

	for (int i = 0;i < 6;i++)
	{
		m_vESCUI.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_Pause_Object" + std::to_wstring(i + 1)));
		m_vESCUI[i]->Enabled(FALSE);
	}

	for (int i = 0;i < 4;i++)
	{
		m_vButton.push_back(LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UI_Player_Pause_Text_Object" + std::to_wstring(i + 1)));
		m_vButton[i]->Enabled(FALSE);
	}

	m_pSelectIcon1 = FIND_OBJECT(L"UI_Player_Puase_SelectIcon1");
	m_pSelectIcon2 = FIND_OBJECT(L"UI_Player_Puase_SelectIcon2");

	m_pSelectIcon1->Enabled(FALSE);
	m_pSelectIcon2->Enabled(FALSE);

	m_pMainLight = FIND_OBJECT(L"MainLight");

	if (LevelMgr::GetInst()->GetCurLevel()->GetName() == L"TownScene" || LevelMgr::GetInst()->GetCurLevel()->GetName() == L"DungeonEntranceScene")
	{

	}
	else
	{
		m_pTopWall = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Room_Structure_Wall_Top");
		m_pBottomWall = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Room_Structure_Wall_Bottom");
		m_pLeftWall = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Room_Structure_Wall_Left");
		m_pRightWall = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Object_Room_Structure_Wall_Right");

		assert(m_pTopWall);
		assert(m_pBottomWall);
		assert(m_pLeftWall);
		assert(m_pRightWall);
	}

	if (L"DungeonEntranceScene" == LevelMgr::GetInst()->GetCurLevel()->GetName())
	{
		assert(m_pDungeonPortal1);
		m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Entrance_Portal1_Close_Anim", FALSE);
	}

	for (int i = 0;i < 20;i++)
	{
		assert(m_pInventorySlotObjs[i]);
	}

	/*for (int i = 0;i < 28;i++)
	{
		assert(m_pChestSlotObjs[i]);
	}*/

	for (int i = 0;i < 8;i++)
	{
		assert(m_pEquipSlotObjs[i]);
	}

	m_pSpearCollider = FIND_OBJECT(L"Player_Collider_Spear");
	m_pOneHandCollider;

	assert(m_pInventoryInface);
	assert(m_pChestIntreface);
	assert(m_pUICurSor);
	assert(m_pPlayerCam);

	assert(m_pSpearCollider);
	//assert(m_pOneHandCollider);
	assert(m_pObject_Village_Fade_BG);
	assert(m_pInventoryInterface2);
	assert(m_pMainLight);

	m_pUICurSor->Enabled(FALSE);
	m_pSpearCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

	m_pPlayerPref = RecordManager::GetInst()->GetCurrentPlayerPref();
	m_pObject_Village_Fade_BG->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(COLOR, Vec4(1.f, 1.f, 1.f, 0.5f));

	m_pInventoryInterface2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(0.f, -900.f, -5.f, 0.f));
	m_pObject_Village_Fade_BG->Enabled(FALSE);

	for (int i = 0;i < 20;i++)
	{
		m_pInventorySlotObjs[i]->Enabled(FALSE);
	}

	/*for (int i = 0;i < 28;i++)
	{
		m_pChestSlotObjs[i]->Enabled(FALSE);
	}*/

	for (int i = 0;i < 8;i++)
	{
		m_pEquipSlotObjs[i]->Enabled(FALSE);
	}

	m_pSelectIcon1->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));

	m_pSelectIcon2->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->
		GetMaterial()->SetMaterialParam(COLOR, Vec4(20.f / 255.f, 157.f / 255.f, 114.f / 255.f, 1.f));


	// 낮 1 1 1
	// 밤 0.118712 0.225286 0.643833

	if (L"GolemDungeonScene" == LevelMgr::GetInst()->GetCurLevel()->GetName() || L"GolemDungeonBossScene" == LevelMgr::GetInst()->GetCurLevel()->GetName())
	{
		m_pMainLight->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		if (m_pPlayerPref->_iTime == 0)
		{
			m_pMainLight->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(0.118712f, 0.225286f, 0.643833f, 1.f));
		}
		else
		{
			m_pMainLight->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D)->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.f));
		}
	}
}

void PlayerScript::Start()
{
	Object::FadeInLightAmbient(m_pMainLight, 2.f);
	IdleAnim();
}

void PlayerScript::Update()
{
	if (nullptr == m_pPlayerPref)
		return;

	if (m_pPlayerPref->_iCurHp <= 0 && m_eState != State::Dead)
	{
		m_eState = State::Dead;
		DeadAnim();
	}

	if (m_bInvincibility && State::Dead != m_eState)
		Invincibility();

	switch (m_eState)
	{
	case State::Dead:
	{
		Dead();
	}
	break;
	case State::Idle:
	{
		Idle();
	}
	break;
	case State::Attack:
	{
		Attack();
	}
	break;
	case State::Chest:
	{
		Chest();
	}
	break;
	case State::CameraMove:
	{
		CameraMove();
	}
	break;
	case State::Move:
	{
		Move();
	}
	break;
	case State::Pause:
	{
		Pause();
	}
	break;
	case State::Dodge:
	{
		Dodge();
	}
	break;
	case State::Scene:
	{
		Scene();
	}
	break;
	case State::SceneChange:
	{
		SceneChange();
	}
	break;
	case State::DungeonEnter:
	{
		DungeonEnter();
	}
	break;
	case State::Inventory:
	{
		Inventory();
	}
	break;
	case State::ESC:
	{
		ESC();
	}
	break;
	case State::None:
		break;
	default:
		break;
	}

	ColliderPositionCalc();
}

void PlayerScript::Invincibility()
{
	m_fInvincibilityTime += DT;
	m_fGliterTime += DT;

	if (m_bFalling && m_fInvincibilityTime >= 1.f)
	{
		m_bFalling = FALSE;
		Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		m_fInvincibilityTime = 0.f;
		_pTr->SetRelativePosition(m_vPrevPos);
		m_pPlayerPref->_iCurHp -= 20;
		Object::ShakingEffect(1.f, 50.f, 2.f);
	}
	else if (!m_bFalling)
	{
		if (m_bWhiteGliter)
		{
			GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 1);
			GetOwner()->GetComponent< Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(SPECULAR, Vec4(1.f, 0.f, 0.f, 1.f));
			m_bWhiteGliter = FALSE;
			m_bRedGliter = TRUE;
			m_fInvincibilityTime = 0.f;
		}
		else if (m_bRedGliter)
		{
			GetOwner()->GetComponent< Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(SPECULAR, Vec4(1.f, 1.f, 1.f, 1.f));
			m_bRedGliter = FALSE;
			m_fInvincibilityTime = 0.f;
		}
		else if (!m_bRedGliter && !m_bWhiteGliter)
		{
			if (m_fGliterTime > 0.2f && !m_bFalling)
			{
				if (0 == m_iGliterFlag)
				{
					m_iGliterFlag = 1;
					GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 1);
				}
				else
				{
					m_iGliterFlag = 0;
					GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 0);
					GetOwner()->GetComponent< Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(SPECULAR, Vec4(0.f, 0.f, 0.f, 0.f));
				}
				m_fGliterTime = 0.f;
			}

			if (m_fInvincibilityTime >= 2.f)
			{
				m_fInvincibilityTime = 0.f;
				m_bInvincibility = FALSE;
				m_iGliterFlag = 0;
				m_fGliterTime = 0.f;
				GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
				GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_2, 0);
				GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_1, 0);
				GetOwner()->GetComponent< Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(SPECULAR, Vec4(1.f, 1.f, 1.f, 1.f));
			}
		}
	}
}

void PlayerScript::Idle()
{
	if (KEY_TAP(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D)
		|| KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::D))
	{
		m_eState = State::Move;
		MoveAnim();
	}
	else if (KEY_TAP(KEY::J))
	{
		m_eState = State::Attack;

		Weapon		_eWaepon = m_pPlayerPref->_eWeapon;
		SubWeapon	_eSunWeapon = m_pPlayerPref->_eSubWeapon;

		Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

		switch (_eWaepon)
		{
		case Weapon::BasicOneHand:
		{

		}
		break;
		case Weapon::BasicTwoHand:
		{

		}
		break;
		case Weapon::BroomStick:
		{
			m_pSpearCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
			switch (m_eDir)
			{
			case Dir::UP:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Up_Anim1", FALSE);
			}
			break;
			case Dir::DOWN:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Down_Anim1", FALSE);
			}
			break;
			case Dir::LEFT:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Left_Anim1", FALSE);
			}
			break;
			case Dir::RIGHT:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Right_Anim1", FALSE);
			}
			break;
			case Dir::None:
				break;
			default:
				break;
			}
		}
		break;
		case Weapon::None:
			break;
		default:
			break;
		}
	}
	else if (KEY_TAP(KEY::SPACE))
	{
		m_eState = State::Dodge;
		m_bInvincibility = TRUE;
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
		DodgeAnim();
	}
	else if (KEY_TAP(KEY::I))
	{
		m_eState = State::Inventory;

		m_pUICurSor->Enabled(TRUE);

		for (int i = 0;i < 20;i++)
		{
			m_pInventorySlotObjs[i]->Enabled(TRUE);
		}

		for (int i = 0;i < 8;i++)
		{
			m_pEquipSlotObjs[i]->Enabled(TRUE);
		}

		Object::MoveUp(m_pInventoryInterface2, 0.f, 2000.f);

		m_pObject_Village_Fade_BG->Enabled(TRUE);

		m_bInventoryOpen = TRUE;
	}
	else if (KEY_TAP(KEY::ESC))
	{
		m_eState = State::ESC;
		m_pObject_Village_Fade_BG->Enabled(TRUE);

		for (int i = 0;i < 6;i++)
		{
			m_vESCUI[i]->Enabled(TRUE);
		}

		for (int i = 0;i < 4;i++)
		{
			m_vButton[i]->Enabled(TRUE);
		}

		m_pSelectIcon1->Enabled(TRUE);
		m_pSelectIcon2->Enabled(TRUE);
	}
}

void PlayerScript::Attack()
{
	// 데미지는 장비에 따라 바뀜
	// 적용 시켜줌
	auto		_iStrikingPower = m_pPlayerPref->_iStrikingPower;
	Weapon		_eWaepon = m_pPlayerPref->_eWeapon;
	SubWeapon	_eSunWeapon = m_pPlayerPref->_eSubWeapon;

	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	if (_pAnim->GetCurAnimation2D()->IsFinish())
	{
		if (m_iCurComboCount < m_iComboCount)
		{
			if (m_iCurComboCount < 3)
			{
				m_iCurComboCount++;

				switch (_eWaepon)
				{
				case Weapon::BasicOneHand:
				{

				}
				break;
				case Weapon::BasicTwoHand:
				{

				}
				break;
				case Weapon::BroomStick:
				{
					m_pSpearCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
					switch (m_eDir)
					{
					case Dir::UP:
					{
						_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Up_Anim" + std::to_wstring(m_iCurComboCount), FALSE);
					}
					break;
					case Dir::DOWN:
					{
						_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Down_Anim" + std::to_wstring(m_iCurComboCount), FALSE);
					}
					break;
					case Dir::LEFT:
					{
						_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Left_Anim" + std::to_wstring(m_iCurComboCount), FALSE);
					}
					break;
					case Dir::RIGHT:
					{
						_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Right_Anim" + std::to_wstring(m_iCurComboCount), FALSE);
					}
					break;
					case Dir::None:
						break;
					default:
						break;
					}
				}
				break;
				case Weapon::None:
					break;
				default:
					break;
				}
			}
		}
		else
		{
			m_iCurComboCount = 1;
			m_iComboCount = 1;
			IdleAnim();
			m_eState = State::Idle;
			m_pSpearCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			return;
		}
	}
	else
	{
		m_fComboTime += DT;
		if (KEY_TAP(J))
		{
			if (m_iComboCount < 3)
			{
				m_iComboCount++;
			}
		}

		if (m_fComboTime >= 0.4f)
		{
			m_pSpearCollider->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
			m_fComboTime = 0.f;
		}
	}
}

void PlayerScript::Move()
{
	Transform* _pTransform = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = nullptr;

	assert(_pTransform);

	// Speed는 장비에 따라 바뀜
	// 적용 시켜줌

	vector<float> _fSpeed_Move(4, 0);

	for (int i = 0;i < 4;i++)
	{
		if (!m_vEnterWall_Move[i])
		{
			_fSpeed_Move[i] = m_pPlayerPref->_fSpeed;
		}
	}

	Vec4 _pos = _pTransform->GetRelativePosition();

	if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D))
	{
		if (KEY_RELEASED(KEY::D))
		{
			m_eDir = Dir::LEFT;
			MoveAnim();
		}

		if (KEY_RELEASED(KEY::A))
		{

			m_eDir = Dir::RIGHT;
			MoveAnim();
		}
	}

	if (KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::S))
	{
		if (KEY_RELEASED(KEY::W))
		{
			m_eDir = Dir::DOWN;
			MoveAnim();
		}

		if (KEY_RELEASED(KEY::S))
		{
			m_eDir = Dir::UP;
			MoveAnim();
		}
	}

	if (KEY_TAP(KEY::W) || KEY_TAP(KEY::S) || KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
	{
		if (KEY_TAP(KEY::W))
		{
			MoveAnim();
		}

		if (KEY_TAP(KEY::S))
		{
			MoveAnim();
		}

		if (KEY_RELEASED(KEY::W))
		{
			if (KEY_PRESSED(KEY::A))
			{
				m_eDir = Dir::LEFT;
			}
			else if (KEY_PRESSED(KEY::D))
			{
				m_eDir = Dir::RIGHT;
			}

			MoveAnim();
		}

		if (KEY_RELEASED(KEY::S))
		{
			if (KEY_PRESSED(KEY::A))
			{
				m_eDir = Dir::LEFT;
			}
			else if (KEY_PRESSED(KEY::D))
			{
				m_eDir = Dir::RIGHT;
			}

			MoveAnim();
		}
	}

	if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D) || KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::S))
	{
		if (KEY_PRESSED(KEY::W))
		{
			_pos.y += _fSpeed_Move[0] * DT;
		}

		if (KEY_PRESSED(KEY::S))
		{
			_pos.y -= _fSpeed_Move[1] * DT;
		}

		if (KEY_PRESSED(KEY::A))
		{
			_pos.x -= _fSpeed_Move[2] * DT;
		}

		if (KEY_PRESSED(KEY::D))
		{
			_pos.x += _fSpeed_Move[3] * DT;
		}
	}
	else
	{
		m_eState = State::Idle;
		IdleAnim();
	}

	if (KEY_TAP(KEY::J))
	{
		m_eState = State::Attack;

		Weapon		_eWaepon = m_pPlayerPref->_eWeapon;
		SubWeapon	_eSunWeapon = m_pPlayerPref->_eSubWeapon;

		Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

		switch (_eWaepon)
		{
		case Weapon::BasicOneHand:
		{

		}
		break;
		case Weapon::BasicTwoHand:
		{

		}
		break;
		case Weapon::BroomStick:
		{
			switch (m_eDir)
			{
			case Dir::UP:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Up_Anim1", FALSE);
			}
			break;
			case Dir::DOWN:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Down_Anim1", FALSE);
			}
			break;
			case Dir::LEFT:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Left_Anim1", FALSE);
			}
			break;
			case Dir::RIGHT:
			{
				_pAnim->Play(L"FSM_Player_Dungeon_Attack_Spear_Right_Anim1", FALSE);
			}
			break;
			case Dir::None:
				break;
			default:
				break;
			}
		}
		break;
		case Weapon::None:
			break;
		default:
			break;
		}
	}

	if (KEY_TAP(SPACE))
	{
		m_eState = State::Dodge;
		DodgeAnim();
	}
	_pos.z = _pos.y;
	_pTransform->SetRelativePosition(_pos);
}

void PlayerScript::Dodge()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
	Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	assert(_pAnim || _pTr);

	Vec4 _vPos = _pTr->GetRelativePosition();

	vector<float>_vSpeed_Dodge(4, 0);
	vector<float>_vSpeed_Dodge_Base;
	_vSpeed_Dodge_Base.push_back(250.f);
	_vSpeed_Dodge_Base.push_back(250.f);
	_vSpeed_Dodge_Base.push_back(500.f);
	_vSpeed_Dodge_Base.push_back(500.f);

	for (int i = 0;i < 4;i++)
	{
		if (!m_vEnterWall_Dodge[i])
		{
			_vSpeed_Dodge[i] = _vSpeed_Dodge_Base[i];
		}
	}

	switch (m_eDir)
	{
	case Dir::UP:
	{
		_vPos.y += _vSpeed_Dodge[0] * DT;
	}
	break;
	case Dir::DOWN:
	{
		_vPos.y -= _vSpeed_Dodge[1] * DT;
	}
	break;
	case Dir::LEFT:
	{
		_vPos.x -= _vSpeed_Dodge[2] * DT;
	}
	break;
	case Dir::RIGHT:
	{
		_vPos.x += _vSpeed_Dodge[3] * DT;
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}

	_pTr->SetRelativePosition(_vPos);

	if (_pAnim->GetCurAnimation2D()->IsFinish())
	{
		m_eState = State::Idle;
		IdleAnim();
		m_bInvincibility = FALSE;
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
	}
}

void PlayerScript::Chest()
{
	m_pObject_Village_Fade_BG->Enabled(TRUE);
	ITEM* _pChest = nullptr;

	if (KEY_TAP(KEY::J))
	{
		m_bChestOpen = TRUE;

		// Initialize
		if (_pChest != nullptr)
		{
			_pChest = m_pChestObject->GetScript<TriggerScript>(L"TriggerScript")->GetChestItem();
		}

		// UI Animation
		GUI_LineScript* _pInventoryScript = m_pInventoryInface->GetScript<GUI_LineScript>(L"GUI_LineScript");
		_pInventoryScript->SetEvent(m_vOutInventoryPos, m_vInInventorypos, 4000.f, GUI_STYLE::MOVEUP);

		GUI_LineScript* _pChsetScript = m_pChestIntreface->GetScript<GUI_LineScript>(L"GUI_LineScript");
		_pChsetScript->SetEvent(m_vOutChestPos, m_vInChestPos, 4000.f, GUI_STYLE::MOVELEFT);

		m_pUICurSor->Enabled(TRUE);
		for (int i = 0;i < 20;i++)
		{
			if (nullptr != m_pInventorySlotObjs[i])
				m_pInventorySlotObjs[i]->Enabled(TRUE);
		}

		for (int i = 0;i < 28;i++)
		{
			if (nullptr != m_pChestSlotObjs[i])
				m_pChestSlotObjs[i]->Enabled(TRUE);
		}
		m_pObject_Village_Fade_BG->Enabled(TRUE);
	}

	if (KEY_TAP(KEY::I))
	{
		m_pObject_Village_Fade_BG->Enabled(FALSE);
		m_bChestOpen = FALSE;

		// UI Animation
		GUI_LineScript* _pInventoryScript = m_pInventoryInface->GetScript<GUI_LineScript>(L"GUI_LineScript");
		_pInventoryScript->SetEvent(m_vInInventorypos, m_vOutInventoryPos, 4000.f, GUI_STYLE::MOVEDOWN);

		GUI_LineScript* _pChsetScript = m_pChestIntreface->GetScript<GUI_LineScript>(L"GUI_LineScript");
		_pChsetScript->SetEvent(m_vInChestPos, m_vOutChestPos, 4000.f, GUI_STYLE::MOVERIGHT);

		m_pUICurSor->Enabled(FALSE);
		for (int i = 0;i < 20;i++)
		{
			if (nullptr != m_pInventorySlotObjs[i])
				m_pInventorySlotObjs[i]->Enabled(FALSE);
		}

		for (int i = 0;i < 28;i++)
		{
			if (nullptr != m_pChestSlotObjs[i])
				m_pChestSlotObjs[i]->Enabled(FALSE);
		}

		m_eState = State::Idle;
	}

	// Chest Open
	if (m_bChestOpen)
	{
		// Control
		if (KEY_TAP(KEY::W))
		{
			if (m_vChestPos[0] > 0)
			{
				m_vChestPos[0] -= 1;
			}
		}

		if (KEY_TAP(KEY::S))
		{
			if (m_vChestPos[0] < 3)
			{
				m_vChestPos[0] += 1;
			}
		}

		if (KEY_TAP(KEY::A))
		{
			if (m_vChestPos[1] > 0)
			{
				m_vChestPos[1] -= 1;
			}
		}

		if (KEY_TAP(KEY::D))
		{
			if (m_vChestPos[1] < 11)
			{
				m_vChestPos[1] += 1;
			}
		}

		if (KEY_TAP(KEY(J)))
		{
			// 아이템 집기
		}

		Vec2 _vInventoryStartPos = Vec2(-505.f, 227.f);
		Vec2 _vChestStartPos = Vec2(70.f, 81.f);

		Vec2 _vInventoryScale = Vec2(90.f, 90.f);
		Vec2 _vInventoryScale2 = Vec2(90.f, 108.f);
		Vec2 _vChestScale = Vec2(88.f, 89.f);

		/**********************
		|	Show Inventory & Chest
		**********************/
		for (int y = 0;y < 4;y++)
		{
			for (int x = 0;x < 5;x++)
			{
				// Inventory
				if (m_pPlayerPref->_eInventory[4 * y + x] != ITEM::NO_ITEM)
				{
					if (m_pInventorySlotObjs[4 * y + x] != nullptr)
					{
						m_pInventorySlotObjs[4 * y + x]->Enabled(TRUE);
						m_pInventorySlotObjs[4 * y + x]->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)
							->GetMaterial()->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(m_vItemName[UINT(m_pPlayerPref->_eInventory[4 * y + x])]));
					}
				}

				// Chest
				/*if (nullptr != _pChest)
				{
					if (_pChest[4 * y + (x - 5)] != ITEM::NO_ITEM)
					{
						if (m_pInventorySlotObjs[4 * y + x] != nullptr)
						{
							m_pInventorySlotObjs[4 * y + x]->Enabled(TRUE);
							m_pInventorySlotObjs[4 * y + x]->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)
								->GetMaterial()->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(m_vItemName[UINT(_pChest[4 * y + (x - 5)])]));
						}
					}
				}*/
			}
		}

		/**********************
		|	Show UICursor
		**********************/
		// Inventory
		Transform* _pUITr = m_pUICurSor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vUIPos = _pUITr->GetRelativePosition();

		if (m_vChestPos[1] < 5)
		{
			_vUIPos.x = m_pInventorySlotObjs[4 * m_vChestPos[0] + m_vChestPos[1]]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().x;
			_vUIPos.y = m_pInventorySlotObjs[4 * m_vChestPos[0] + m_vChestPos[1]]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y;
		}
		// Chest
		else
		{
			_vUIPos.x = m_pChestSlotObjs[4 * m_vChestPos[0] + (m_vChestPos[1] - 5)]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().x;
			_vUIPos.y = m_pChestSlotObjs[4 * m_vChestPos[0] + (m_vChestPos[1] - 5)]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y;
		}

		_pUITr->SetRelativePosition(_vUIPos);
		_pUITr->SetRelativeScale(Vec4(100.f, 100.f, 1.f, 1.f));
		Object::DecreaseEffect(m_pUICurSor, 80.f, 600.f);
	}
	else
	{
		if (KEY_TAP(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D)
			|| KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::D))
		{
			MoveAnim();
		}
		else if (KEY_TAP(KEY::SPACE))
		{
			m_bInvincibility = TRUE;
			DodgeAnim();
		}
	}
}

void PlayerScript::SceneChange()
{
	if (!m_bSceneChange)
	{
		m_bSceneChange = TRUE;
		Object::FadeOutLightAmbient(m_pMainLight, 1.5f);
	}
	else
	{
		m_fSceneChangeTime += DT;

		if (m_fSceneChangeTime >= 1.5f)
		{
			Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			switch (m_eSceneTrigger)
			{
			case TRIGGER_TYPE::DUNGEONENTRANCE:
			{
				_vPos.y -= 100.f;
				GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);
				LevelMgr::GetInst()->SelectLevel(L"DungeonEntranceScene");
			}
			break;
			case TRIGGER_TYPE::TOWN:
			{
				_vPos.y += 100.f;
				GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);
				LevelMgr::GetInst()->SelectLevel(L"TownScene");
			}
			break;
			case TRIGGER_TYPE::WILLROOM:
			{
				//vPos.y += 100.f;
				//GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);
				//LevelMgr::GetInst()->SelectLevel(L"DungeonEntranceScene");
			}
			break;
			case TRIGGER_TYPE::GOLEM_BOSS_MOVE_TRIGGER:
			{
				GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);
				LevelMgr::GetInst()->SelectLevel(L"GolemDungeonBossScene");
			}
			break;
			case TRIGGER_TYPE::END:
				break;
			default:
				break;
			}

			m_eState = State::Idle;
			m_bSceneChange = FALSE;
			m_fSceneChangeTime = 0.f;

		}
	}
}

void PlayerScript::Scene()
{
	int _iFrame = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetCurFrame();

	if ((_iFrame >= 37 && _iFrame <= 60) || _iFrame >= 96)
		return;

	Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vPos = _pTr->GetRelativePosition();

	float _fSpeed = 60.f;

	_vPos.x += _fSpeed * DT;

	_pTr->SetRelativePosition(_vPos);
}

void PlayerScript::Inventory()
{
	if (KEY_TAP(KEY::I))
	{
		m_eState = State::Idle;
		m_bInventoryOpen = !m_bInventoryOpen;
		m_pObject_Village_Fade_BG->Enabled(FALSE);
		//UI Animation
		m_pUICurSor->Enabled(FALSE);

		for (int i = 0;i < 20;i++)
		{
			m_pInventorySlotObjs[i]->Enabled(FALSE);
		}

		for (int i = 0;i < 8;i++)
		{
			m_pEquipSlotObjs[i]->Enabled(FALSE);
		}

		Object::MoveDown(m_pInventoryInterface2, -1000.f, 2000.f);
	}

	if (m_bInventoryOpen)
	{
		m_pUICurSor->Enabled(TRUE);

		Transform* _pUITr = m_pUICurSor->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		Vec4 _vUIPos = _pUITr->GetRelativePosition();

		// Control
		if (KEY_TAP(KEY::W))
		{
			if (m_vChestPos[0] > 0)
			{
				m_vChestPos[0] -= 1;
				_pUITr->SetRelativeScale(Vec4(100.f, 100.f, 1.f, 1.f));
				Object::DecreaseEffect(m_pUICurSor, 80.f, 600.f);
			}
		}

		if (KEY_TAP(KEY::S))
		{
			if (m_vChestPos[0] < 3)
			{
				m_vChestPos[0] += 1;
				_pUITr->SetRelativeScale(Vec4(100.f, 100.f, 1.f, 1.f));
				Object::DecreaseEffect(m_pUICurSor, 80.f, 600.f);
			}
		}

		if (KEY_TAP(KEY::A))
		{
			if (m_vChestPos[1] > 0)
			{
				m_vChestPos[1] -= 1;
				_pUITr->SetRelativeScale(Vec4(100.f, 100.f, 1.f, 1.f));
				Object::DecreaseEffect(m_pUICurSor, 80.f, 600.f);
			}
		}

		if (KEY_TAP(KEY::D))
		{
			if (m_vChestPos[1] < 11)
			{
				m_vChestPos[1] += 1;
				_pUITr->SetRelativeScale(Vec4(100.f, 100.f, 1.f, 1.f));
				Object::DecreaseEffect(m_pUICurSor, 80.f, 600.f);
			}
		}

		if (KEY_TAP(KEY(J)))
		{
			// 아이템 집기
		}
		else
		{
			// 아이템 놓기
		}

		Vec2 _vInventoryStartPos = Vec2(0.f, 0.f);
		Vec2 _vChestStartPos = Vec2(0.f, 0.f);

		Vec2 _vInventoryScale = Vec2(0.f, 0.f);
		Vec2 _vChestScale = Vec2(0.f, 0.f);

		/**********************
		|	Show Inventory & Chest
		**********************/
		for (int y = 0;y < 4;y++)
		{
			for (int x = 0;x < 5;x++)
			{
				// Inventory
				if (m_pPlayerPref->_eInventory[5 * y + x] != ITEM::NO_ITEM)
				{
					m_pInventorySlotObjs[5 * y + x]->Enabled(TRUE);
					m_pInventorySlotObjs[5 * y + x]->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)
						->GetMaterial()->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(m_vItemName[UINT(m_pPlayerPref->_eInventory[4 * y + x])]));
				}
			}
		}

		for (int y = 0;y < 4;y++)
		{
			for (int x = 0;x < 2;x++)
			{
				// Equip
				if (m_pPlayerPref->_eInventory[2 * y + x] != ITEM::NO_ITEM)
				{
					m_pInventorySlotObjs[2 * y + x]->Enabled(TRUE);
					m_pInventorySlotObjs[2 * y + x]->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)
						->GetMaterial()->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(m_vItemName[UINT(m_pPlayerPref->_eInventory[2 * y + x])]));
				}
			}
		}
		/**********************
		|	Show UICursor
		**********************/
		// Inventory

		if (m_vChestPos[1] < 5)
		{
			_vUIPos.x = m_pInventorySlotObjs[5 * m_vChestPos[0] + m_vChestPos[1]]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().x;
			_vUIPos.y = m_pInventorySlotObjs[5 * m_vChestPos[0] + m_vChestPos[1]]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y;
		}
		// Chest
		else
		{
			_vUIPos.x = m_pEquipSlotObjs[2 * m_vChestPos[0] + (m_vChestPos[1] - 5)]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().x;
			_vUIPos.y = m_pEquipSlotObjs[2 * m_vChestPos[0] + (m_vChestPos[1] - 5)]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition().y;
		}

		// UI Animation
		_pUITr->SetRelativePosition(_vUIPos);
	}
}

void PlayerScript::DungeonEnter()
{
	if (m_bSceneChange)
	{
		switch (m_eSceneTrigger)
		{
		case TRIGGER_TYPE::GOLEM_DUNGEON:
		{
			if (m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish())
			{
				Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
				Object::FadeOutLightAmbient(m_pMainLight, 1.5f);
				_vPos.y -= 100.f;
				GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(_vPos);
				GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativePosition(Vec4(0.f, 0.f, 0.f, 1.f));
				m_bSceneChange = FALSE;
			}
		}
		break;
		case TRIGGER_TYPE::END:
			break;
		default:
			break;
		}
	}
	else
	{
		m_fDungeonEnterTime += DT;

		if (m_fDungeonEnterTime >= 1.5f)
		{
			LevelMgr::GetInst()->SelectLevel(L"GolemDungeonScene");

			GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->SetIsRender(TRUE);
			m_bSceneChange = TRUE;
			m_fDungeonEnterTime = 0.f;
			m_eState = State::Idle;
		}
	}
}

void PlayerScript::CameraMove()
{
	m_eState = State::Pause;

	GameObject* _pMainCam = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera");
	Transform* _pCamTr = _pMainCam->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vStartPos = _pCamTr->GetRelativePosition();
	Vec4 _vDestPos = _vStartPos;

	Transform* _pPlayerTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vPlayerPos = _pPlayerTr->GetRelativePosition();
	_vPlayerPos.x = _vStartPos.x;
	_vPlayerPos.y = _vStartPos.y;

	Transform* _pTopWallTr = m_pTopWall->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _pBottomWallTr = m_pBottomWall->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _pLeftWallTr = m_pLeftWall->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _pRightWallTr = m_pRightWall->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

	Vec4 _vTopWallPos = _pTopWallTr->GetRelativePosition();
	Vec4 _vBottomWallPos = _pBottomWallTr->GetRelativePosition();
	Vec4 _vLeftWallPos = _pLeftWallTr->GetRelativePosition();
	Vec4 _vRightWallPos = _pRightWallTr->GetRelativePosition();
	switch (m_eDir)
	{
	case Dir::UP:
	{
		_vDestPos.y += 900.f;
		_vPlayerPos.y = _vDestPos.y;
		_vPlayerPos.y -= 350.f;

		_vTopWallPos.y += 900.f;
		_vBottomWallPos.y += 900.f;
		_vLeftWallPos.y += 900.f;
		_vRightWallPos.y += 900.f;

		Object::MoveUp(_pMainCam, _vDestPos.y, 2000.f);

		Content::Vec2 _vPos = RoomManager::GetInst()->GetCurPos();
		_vPos.y -= 1;
		RoomManager::GetInst()->SetCurPos(_vPos);
	}
	break;
	case Dir::DOWN:
	{
		_vDestPos.y -= 900.f;
		_vPlayerPos.y = _vDestPos.y;
		_vPlayerPos.y += 350.f;

		_vTopWallPos.y -= 900.f;
		_vBottomWallPos.y -= 900.f;
		_vLeftWallPos.y -= 900.f;
		_vRightWallPos.y -= 900.f;

		Object::MoveDown(_pMainCam, _vDestPos.y, 2000.f);

		Content::Vec2 _vPos = RoomManager::GetInst()->GetCurPos();
		_vPos.y += 1;
		RoomManager::GetInst()->SetCurPos(_vPos);
	}
	break;
	case Dir::LEFT:
	{
		_vDestPos.x -= 1600.f;
		_vPlayerPos.x = _vDestPos.x;
		_vPlayerPos.x += 600.f;

		_vTopWallPos.x -= 1600.f;
		_vBottomWallPos.x -= 1600.f;
		_vLeftWallPos.x -= 1600.f;
		_vRightWallPos.x -= 1600.f;

		Object::MoveLeft(_pMainCam, _vDestPos.x, 2000.f);

		Content::Vec2 _vPos = RoomManager::GetInst()->GetCurPos();
		_vPos.x -= 1;
		RoomManager::GetInst()->SetCurPos(_vPos);
	}
	break;
	case Dir::RIGHT:
	{
		_vDestPos.x += 1600.f;
		_vPlayerPos.x = _vDestPos.x;
		_vPlayerPos.x -= 600.f;

		_vTopWallPos.x += 1600.f;
		_vBottomWallPos.x += 1600.f;
		_vLeftWallPos.x += 1600.f;
		_vRightWallPos.x += 1600.f;

		Object::MoveRight(_pMainCam, _vDestPos.x, 2000.f);

		Content::Vec2 _vPos = RoomManager::GetInst()->GetCurPos();
		_vPos.x += 1;
		RoomManager::GetInst()->SetCurPos(_vPos);
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}

	_pPlayerTr->SetRelativePosition(_vPlayerPos);

	_pTopWallTr->SetRelativePosition(_vTopWallPos);
	_pBottomWallTr->SetRelativePosition(_vBottomWallPos);
	_pLeftWallTr->SetRelativePosition(_vLeftWallPos);
	_pRightWallTr->SetRelativePosition(_vRightWallPos);
}

void PlayerScript::Pause()
{
	m_fAccTime += DT;
	if (m_fAccTime >= m_fPauseTime)
	{
		// Enemy On
		GetOwner()->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D)->Enabled(TRUE);
		m_eState = State::Idle;
		m_fAccTime = 0.f;
	}
}

void PlayerScript::Dead()
{
	// Dead
}

void PlayerScript::ESC()
{
	if (KEY_TAP(W))
	{
		if (m_iCurButton > 0)
		{
			m_iCurButton--;

			Vec4 _vSelectPos = m_vButton[m_iCurButton]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			Vec4 _vSelectIcon2 = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			Vec4 _vSelectIcon1 = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			_vSelectIcon1.x = -150.f;
			_vSelectIcon1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon1.x - 100.f, _vSelectIcon1.y, _vSelectIcon1.z, _vSelectIcon1.w),
				_vSelectIcon1, 500.f, GUI_STYLE::MOVERIGHT);

			_vSelectIcon2.x = 150.f;
			_vSelectIcon2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon2.x + 100.f, _vSelectIcon2.y, _vSelectIcon2.z, _vSelectIcon2.w),
				_vSelectIcon2, 500.f, GUI_STYLE::MOVELEFT);

		}
	}

	if (KEY_TAP(S))
	{
		if (m_iCurButton < 3)
		{
			m_iCurButton++;

			Vec4 _vSelectPos = m_vButton[m_iCurButton]->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			Vec4 _vSelectIcon2 = m_pSelectIcon2->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
			Vec4 _vSelectIcon1 = m_pSelectIcon1->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

			_vSelectIcon1.x = -150.f;
			_vSelectIcon1.y = _vSelectPos.y;
			GUI_LineScript* _script = m_pSelectIcon1->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon1.x - 100.f, _vSelectIcon1.y, _vSelectIcon1.z, _vSelectIcon1.w),
				_vSelectIcon1, 500.f, GUI_STYLE::MOVERIGHT);

			_vSelectIcon2.x = 150.f;
			_vSelectIcon2.y = _vSelectPos.y;
			_script = m_pSelectIcon2->GetScript<GUI_LineScript>(L"GUI_LineScript");
			_script->SetEvent(Vec4(_vSelectIcon2.x + 100.f, _vSelectIcon2.y, _vSelectIcon2.z, _vSelectIcon2.w),
				_vSelectIcon2, 500.f, GUI_STYLE::MOVELEFT);
		}
	}

	if (KEY_TAP(J))
	{
		if (0 == m_iCurButton)
		{
			m_eState = State::Idle;
			IdleAnim();
			m_pObject_Village_Fade_BG->Enabled(FALSE);

			for (int i = 0;i < 6;i++)
			{
				m_vESCUI[i]->Enabled(FALSE);
			}

			for (int i = 0;i < 4;i++)
			{
				m_vButton[i]->Enabled(FALSE);
			}

			m_pSelectIcon1->Enabled(FALSE);
			m_pSelectIcon2->Enabled(FALSE);
		}
		else if (1 == m_iCurButton)
		{
			// Option
		}
		else if (2 == m_iCurButton)
		{
			LevelMgr::GetInst()->SelectLevel(L"EnterScene");
		}
		else if (3 == m_iCurButton)
		{
			PostQuitMessage(0);
		}
	}

	if (KEY_TAP(KEY::ESC))
	{
		m_eState = State::Idle;
		IdleAnim();
		m_pObject_Village_Fade_BG->Enabled(FALSE);

		for (int i = 0;i < 6;i++)
		{
			m_vESCUI[i]->Enabled(FALSE);
		}

		for (int i = 0;i < 4;i++)
		{
			m_vButton[i]->Enabled(FALSE);
		}

		m_pSelectIcon1->Enabled(FALSE);
		m_pSelectIcon2->Enabled(FALSE);
	}
}

void PlayerScript::IdleAnim()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pAnim);

	switch (m_eDir)
	{
	case Dir::UP:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Up_Anim");
	}
	break;
	case Dir::DOWN:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Down_Anim");
	}
	break;
	case Dir::LEFT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Left_Anim");
	}
	break;
	case Dir::RIGHT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Right_Anim");
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}
}

void PlayerScript::AttackAnim()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pAnim);

	switch (m_eDir)
	{
	case Dir::UP:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Up");
	}
	break;
	case Dir::DOWN:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Down");
	}
	break;
	case Dir::LEFT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Left");
	}
	break;
	case Dir::RIGHT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Idle_Right");
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}
}

void PlayerScript::MoveAnim()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pAnim);

	if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
	{
		m_eDir = Dir::LEFT;
		_pAnim->Play(L"FSM_Player_Dungeon_Move_Left_Anim");
	}

	if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
	{
		m_eDir = Dir::RIGHT;
		_pAnim->Play(L"FSM_Player_Dungeon_Move_Right_Anim");
	}

	if (KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W))
	{
		m_eDir = Dir::UP;
		_pAnim->Play(L"FSM_Player_Dungeon_Move_Up_Anim");
	}

	if (KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S))
	{
		m_eDir = Dir::DOWN;
		_pAnim->Play(L"FSM_Player_Dungeon_Move_Down_Anim");
	}
}

void PlayerScript::DodgeAnim()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pAnim);

	switch (m_eDir)
	{
	case Dir::UP:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Dodge_Up_Anim", FALSE);
	}
	break;
	case Dir::DOWN:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Dodge_Down_Anim", FALSE);
	}
	break;
	case Dir::LEFT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Dodge_Left_Anim", FALSE);
	}
	break;
	case Dir::RIGHT:
	{
		_pAnim->Play(L"FSM_Player_Dungeon_Dodge_Right_Anim", FALSE);
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}
}

void PlayerScript::DeadAnim()
{
	Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

	assert(_pAnim);

	_pAnim->Play(L"FSM_Player_Dungeon_Dead_Anim", FALSE);
}

void PlayerScript::ColliderPositionCalc()
{
	Weapon _eWeapon = m_pPlayerPref->_eWeapon;
	Vec4 _vPos = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();

	if (Weapon::None == _eWeapon)
		return;

	switch (m_eDir)
	{
	case Dir::UP:
	{
		if (Weapon::BroomStick == _eWeapon)
		{
			Transform* _pSpearTr = m_pSpearCollider->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pSpearTr->GetRelativeScale();
			_vPos.y += 75.f;

			_vScale.x = 40.f;
			_vScale.y = 50.f;

			_pSpearTr->SetRelativeScale(_vScale);
			_pSpearTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			_pSpearTr->SetRelativePosition(_vPos);
		}
		else if (Weapon::BasicOneHand == _eWeapon)
		{

		}
		else if (Weapon::BasicTwoHand == _eWeapon)
		{

		}
	}
	break;
	case Dir::DOWN:
	{
		if (Weapon::BroomStick == _eWeapon)
		{
			Transform* _pSpearTr = m_pSpearCollider->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pSpearTr->GetRelativeScale();
			_vPos.y -= 90.f;

			_vScale.x = 40.f;
			_vScale.y = 70.f;

			_pSpearTr->SetRelativeScale(_vScale);
			_pSpearTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			_pSpearTr->SetRelativePosition(_vPos);
		}
		else if (Weapon::BasicOneHand == _eWeapon)
		{

		}
		else if (Weapon::BasicTwoHand == _eWeapon)
		{

		}
	}
	break;
	case Dir::LEFT:
	{
		if (Weapon::BroomStick == _eWeapon)
		{
			Transform* _pSpearTr = m_pSpearCollider->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pSpearTr->GetRelativeScale();
			_vPos.x -= 85.f;

			_vScale.x = 35.f;
			_vScale.y = 70.f;

			_pSpearTr->SetRelativeScale(_vScale);
			_pSpearTr->SetRelativeRotation(Vec3(0.f, 0.f, 90.f));
			_pSpearTr->SetRelativePosition(_vPos);
		}
		else if (Weapon::BasicOneHand == _eWeapon)
		{

		}
		else if (Weapon::BasicTwoHand == _eWeapon)
		{

		}
	}
	break;
	case Dir::RIGHT:
	{
		if (Weapon::BroomStick == _eWeapon)
		{
			Transform* _pSpearTr = m_pSpearCollider->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vScale = _pSpearTr->GetRelativeScale();
			_vPos.x += 85.f;

			_vScale.x = 35.f;
			_vScale.y = 70.f;

			_pSpearTr->SetRelativeScale(_vScale);
			_pSpearTr->SetRelativeRotation(Vec3(0.f, 0.f, 90.f));
			_pSpearTr->SetRelativePosition(_vPos);
		}
		else if (Weapon::BasicOneHand == _eWeapon)
		{

		}
		else if (Weapon::BasicTwoHand == _eWeapon)
		{

		}
	}
	break;
	case Dir::None:
		break;
	default:
		break;
	}
}

void PlayerScript::OnTriggerEnter(Collider* _other)
{
	if (nullptr == _other)
		return;

	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		// Trigger 타입에 따라
		if (TRIGGER_TYPE::CHEST_TRIGGER == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			m_eState = State::Chest;
			m_pChestObject = _other->GetOwner();
		}
	}

	if (LAYER_TYPE::STRUCTURE_WALL_TOP == _eType)
	{
		m_vEnterWall_Move[0] = TRUE;
		m_vEnterWall_Dodge[0] = TRUE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_BOTTOM == _eType)
	{
		m_vEnterWall_Move[1] = TRUE;
		m_vEnterWall_Dodge[1] = TRUE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_LEFT == _eType)
	{
		m_vEnterWall_Move[2] = TRUE;
		m_vEnterWall_Dodge[2] = TRUE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_RIGHT == _eType)
	{
		m_vEnterWall_Move[3] = TRUE;
		m_vEnterWall_Dodge[3] = TRUE;
	}

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_HOLE == _eType)
	{
		Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		switch (m_eDir)
		{
		case Dir::UP:
		{
			m_vEnterWall_Move[0] = TRUE;

			m_vPrevPos = _pTr->GetRelativePosition();
			m_vPrevPos.y -= 100.f;
		}
		break;
		case Dir::DOWN:
		{
			m_vEnterWall_Move[1] = TRUE;

			m_vPrevPos = _pTr->GetRelativePosition();
			m_vPrevPos.y += 100.f;
		}
		break;
		case Dir::LEFT:
		{
			m_vEnterWall_Move[2] = TRUE;

			m_vPrevPos = _pTr->GetRelativePosition();
			m_vPrevPos.x += 100.f;
		}
		break;
		case Dir::RIGHT:
		{
			m_vEnterWall_Move[3] = TRUE;

			m_vPrevPos = _pTr->GetRelativePosition();
			m_vPrevPos.x -= 100.f;
		}
		break;
		case Dir::None:
			break;
		default:
			break;
		}
	}

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK == _eType)
	{
		switch (m_eDir)
		{
		case Dir::UP:
		{
			m_vEnterWall_Move[0] = TRUE;
			m_vEnterWall_Dodge[0] = TRUE;
			break;
		}
		case Dir::DOWN:
		{
			m_vEnterWall_Move[1] = TRUE;
			m_vEnterWall_Dodge[1] = TRUE;
		}
		break;
		case Dir::LEFT:
		{
			m_vEnterWall_Move[2] = TRUE;
			m_vEnterWall_Dodge[2] = TRUE;
		}
		break;
		case Dir::RIGHT:
		{
			m_vEnterWall_Move[3] = TRUE;
			m_vEnterWall_Dodge[3] = TRUE;
		}
		break;
		case Dir::None:
			break;
		default:
			break;
		}
	}

	if (LAYER_TYPE::MONSTER == _eType)
	{
		EnemyScript* _pScript = _other->GetOwner()->GetScript<EnemyScript>();

		m_pPlayerPref->_iCurHp -= _pScript->GetDamage();
		m_bInvincibility = TRUE;
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
		GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_2, 1);
		m_bWhiteGliter = TRUE;

		if (m_pPlayerPref->_iCurHp <= 0)
			m_pPlayerPref->_iCurHp = 0;
		Object::ShakingEffect(1.f, 50.f, 2.f);
	}

	if (LAYER_TYPE::ENEMY_PROJECTILE == _eType)
	{
		ProjecTileScript* _pScript = _other->GetOwner()->GetScript<ProjecTileScript>(L"ProjecTileScript");
		m_pPlayerPref->_iCurHp -= _pScript->GetDamage();
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
		m_bInvincibility = TRUE;
		GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_2, 1);
		m_bWhiteGliter = TRUE;

		if (m_pPlayerPref->_iCurHp <= 0)
			m_pPlayerPref->_iCurHp = 0;
		Object::ShakingEffect(1.f, 50.f, 2.f);
	}

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		if (TRIGGER_TYPE::GOLEM_DUNGEON == _other->GetOwner()->GetScript<TriggerScript>()->GetTriggerType())
		{
			m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Entrance_Portal1_Open_Anim", FALSE);
		}
	}
}

void PlayerScript::OnTriggerStay(Collider* _other)
{
	if (nullptr == _other)
		return;

	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_HOLE == _eType)
	{
		m_fClifTime += DT;

		if (m_fClifTime >= 2.5f)
		{
			m_fClifTime = 0.f;
			m_bFalling = TRUE;
			GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);

			Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

			Transform* _pOtherTr = _other->GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vOtherPos = _pOtherTr->GetRelativePosition();
			_vOtherPos.z = m_vPrevPos.z;

			_pTr->SetRelativePosition(_vOtherPos);
			m_bInvincibility = TRUE;
			GetOwner()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->SetMaterialParam(INT_2, 1);
			m_bWhiteGliter = TRUE;
		}
	}

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		if (TRIGGER_TYPE::MOVE_TRIGGER == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			m_fTriggerTime += DT;

			if (m_fTriggerTime >= 1.f)
			{
				m_fTriggerTime = 0.f;
				m_fPauseTime = 1.5f;
				GetOwner()->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D)->Enabled(FALSE);
				m_eState = State::CameraMove;
			}
		}

		if (TRIGGER_TYPE::TOWN == _other->GetOwner()->GetScript<TriggerScript>()->GetTriggerType())
		{
			m_eSceneTrigger = TRIGGER_TYPE::TOWN;
			m_eState = State::SceneChange;
		}

		if (TRIGGER_TYPE::GOLEM_DUNGEON == _other->GetOwner()->GetScript<TriggerScript>()->GetTriggerType())
		{
			if (m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->IsFinish()
				&& L"Object_Entrance_Portal1_Open_Anim" == m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->GetCurAnimation2D()->GetName())
			{
				m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Entrance_Portal1_Cycle_Anim", TRUE);
			}

			if (KEY_TAP(KEY::J))
			{
				m_eSceneTrigger = TRIGGER_TYPE::GOLEM_DUNGEON;
				m_eState = State::DungeonEnter;
				GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->SetIsRender(FALSE);
				m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Entrance_Portal1_Absorb_Anim", FALSE);
			}
		}

		if (TRIGGER_TYPE::DUNGEONENTRANCE == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			m_eSceneTrigger = TRIGGER_TYPE::DUNGEONENTRANCE;
			m_eState = State::SceneChange;
		}

		if (TRIGGER_TYPE::GOLEM_BOSS_MOVE_TRIGGER == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			m_eSceneTrigger = TRIGGER_TYPE::GOLEM_BOSS_MOVE_TRIGGER;
			m_eState = State::SceneChange;
		}

		if (TRIGGER_TYPE::WILLROOM == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			if (KEY_TAP(KEY::J))
			{
				m_eSceneTrigger = TRIGGER_TYPE::WILLROOM;
				Object::FadeOutLightAmbient(m_pMainLight, 1.5f);
				m_eState = State::SceneChange;
			}
		}
	}
}

void PlayerScript::OnTriggerExit(Collider* _other)
{
	if (nullptr == _other)
		return;

	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::STRUCTURE_WALL_TOP == _eType)
	{
		m_vEnterWall_Move[0] = FALSE;
		m_vEnterWall_Dodge[0] = FALSE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_BOTTOM == _eType)
	{
		m_vEnterWall_Move[1] = FALSE;
		m_vEnterWall_Dodge[1] = FALSE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_LEFT == _eType)
	{
		m_vEnterWall_Move[2] = FALSE;
		m_vEnterWall_Dodge[2] = FALSE;
	}

	if (LAYER_TYPE::STRUCTURE_WALL_RIGHT == _eType)
	{
		m_vEnterWall_Move[3] = FALSE;
		m_vEnterWall_Dodge[3] = FALSE;
	}

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_HOLE == _eType)
	{
		for (int i = 0;i < 4;i++)
		{
			m_vEnterWall_Move[i] = FALSE;
		}
		m_fClifTime = 0.f;
	}

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK == _eType)
	{
		for (int i = 0;i < 4;i++)
		{
			m_vEnterWall_Move[i] = FALSE;
			m_vEnterWall_Dodge[i] = FALSE;
		}
	}

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		if (TRIGGER_TYPE::CHEST_TRIGGER == _other->GetOwner()->GetScript<TriggerScript>(L"TriggerScript")->GetTriggerType())
		{
			m_eState = State::Idle;
		}
	}

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		if (TRIGGER_TYPE::GOLEM_DUNGEON == _other->GetOwner()->GetScript<TriggerScript>()->GetTriggerType())
		{
			m_pDungeonPortal1->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D)->Play(L"Object_Entrance_Portal1_Close_Anim", FALSE);
		}
	}
}