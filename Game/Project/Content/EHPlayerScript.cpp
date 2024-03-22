#include "pch.h"
#include "EHPlayerScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHKeyMgr.h"
#include <EHLevelMgr.h>

#include "EHTimeMgr.h"

#include "EHImGUIMgr.h"
#include "EHAssetMgr.h"

#include <EHAnimation2D.h>

PlayerScript::PlayerScript()
	: m_eDir(Dir::LEFT)
	, m_eState(State::Idle)
	, m_pPlayerPref(nullptr)
	, m_fAccTime(0.f)
	, m_fClifTime(0.f)
	, m_bFalling(FALSE)
	, m_vPrevPos{}
	, m_fInvincibilityTime(0.f)
	, m_fTriggerTime(0.f)
	, m_bInvincibility(FALSE)
	, m_vEnterWall_Move(4, FALSE)
	, m_vEnterWall_Dodge(4, FALSE)
{
	SetName(L"PlayerScript");
}

PlayerScript::~PlayerScript()
{
	// 나중에 없애야함
	delete m_pPlayerPref;
}

void PlayerScript::Awake()
{
	// 나중에 없애야함
	m_pPlayerPref = new PlayerPref;
	m_pPlayerPref->_bTutorial = 0;
	m_pPlayerPref->_ePlace = PLACE::TUTORIAL;
	m_pPlayerPref->_iTime = 0;
	m_pPlayerPref->_vPos = Vec2(0.f, 0.f);
	m_pPlayerPref->_iClear = 0;
	m_pPlayerPref->_iCurHp = 100;
	m_pPlayerPref->_iMaxHp = 100;
	m_pPlayerPref->_iMoney = 0;
	m_pPlayerPref->_fSpeed = 300.f;
	m_pPlayerPref->_iStrikingPower = 10;
	m_pPlayerPref->_eWeapon = Weapon::Spear;
	m_pPlayerPref->_eSubWeapon = SubWeapon::None;
	//RecordManager::GetInst()->GetCurrentPlayerPref();
}

void PlayerScript::Start()
{
	IdleAnim();
}

void PlayerScript::Update()
{
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
	case State::Move:
	{
		Move();
	}
	break;
	case State::Dodge:
	{
		Dodge();
	}
	break;
	case State::None:
		break;
	default:
		break;
	}
}

void PlayerScript::Invincibility()
{
	m_fInvincibilityTime += DT;

	if (m_bFalling && m_fInvincibilityTime >= 1.f)
	{
		m_bFalling = FALSE;
		Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		_pTr->SetRelativePosition(m_vPrevPos);
	}

	if (m_fInvincibilityTime >= 2.f)
	{
		m_fInvincibilityTime = 0.f;
		m_bInvincibility = FALSE;
		GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled();
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
	}
	else if (KEY_TAP(KEY::SPACE))
	{
		m_eState = State::Dodge;
		m_bInvincibility = TRUE;
		DodgeAnim();
	}
}

void PlayerScript::Attack()
{
	// 데미지는 장비에 따라 바뀜
	// 적용 시켜줌
	auto		_iStrikingPower = m_pPlayerPref->_iStrikingPower;
	Weapon		_eWaepon = m_pPlayerPref->_eWeapon;
	SubWeapon	_eSunWeapon = m_pPlayerPref->_eSubWeapon;

	m_fAccTime += DT;

	switch (_eWaepon)
	{
	case Weapon::OneHand:
	{

	}
	break;
	case Weapon::TwoHand:
	{

	}
	break;
	case Weapon::Spear:
	{

	}
	break;
	case Weapon::None:
		break;
	default:
		break;
	}

	if (m_fAccTime >= 0.3f)
	{
		m_eState = State::Idle;




		m_fAccTime = 0.f;
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

	if (KEY_TAP(J))
	{
		m_eState = State::Attack;
		AttackAnim();
	}

	if (KEY_TAP(SPACE))
	{
		m_eState = State::Dodge;
		DodgeAnim();
	}

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
	}
}

void PlayerScript::Dead()
{
	// Dead
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

void PlayerScript::OnTriggerEnter(Collider* _other)
{
	if (nullptr == _other)
		return;

	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

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

	if (LAYER_TYPE::STRUCTURE_OBSTACLE == _eType)
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
}

void PlayerScript::OnTriggerStay(Collider* _other)
{
	if (nullptr == _other)
		return;

	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::STRUCTURE_OBSTACLE == _eType)
	{
		m_fClifTime += DT;

		if (m_fClifTime >= 2.5f)
		{
			m_fClifTime = 0.f;
			m_bFalling = TRUE;
			GetOwner()->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D)->Enabled();

			Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

			Transform* _pOtherTr = _other->GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
			Vec4 _vOtherPos = _pOtherTr->GetRelativePosition();

			_pTr->SetRelativePosition(_vOtherPos);
			m_bInvincibility = TRUE;
		}
	}

	if (LAYER_TYPE::TRIGGER == _eType)
	{
		m_fTriggerTime += DT;

		if (m_fTriggerTime >= 1.f)
		{
			m_fTriggerTime = 0.f;
			// 카메라 이동
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

	if (LAYER_TYPE::STRUCTURE_OBSTACLE == _eType)
	{
		for (int i = 0;i < 4;i++)
		{
			m_vEnterWall_Move[i] = FALSE;
		}
	}
}