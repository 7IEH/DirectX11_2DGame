#pragma once
#include "EHScript.h"
#include "EHRecordManager.h"

#include "Content_enum.h"

struct ComboTask
{
	int _iCombo;
};

enum class	State
{
	Idle,
	Attack,
	Move,
	Dodge,
	Chest,
	Inventory,
	CameraMove,
	Dead,
	Pause,
	ESC,
	Scene,
	SceneChange,
	DungeonEnter,
	None,
};

class PlayerScript
	:public Script
{
private:
	/**************************
	|	Basic Member
	**************************/
	Dir					m_eDir;
	State				m_eState;
	PlayerPref* m_pPlayerPref;

	float				m_fAccTime;

	vector<bool>		m_vEnterWall_Move;
	vector<bool>		m_vEnterWall_Dodge;

	GameObject* m_pMozaicPanel;

	float				m_fDeadTime;
	bool				m_bMozaikEnd;

	/**************************
	|	Falling Member
	**************************/
	Vec4				m_vPrevPos;
	float				m_fClifTime;
	bool				m_bFalling;

	/**************************
	|	¹«Àû Member
	**************************/
	float				m_fInvincibilityTime;
	float				m_fGliterTime;
	int					m_iGliterFlag;

	bool				m_bWhiteGliter;
	bool				m_bRedGliter;
	bool				m_bInvincibility;

	/**************************
	|	Trigger Member
	|	1. Chest
	|	2. Inventory
	**************************/
	float				m_fTriggerTime;
	bool				m_bInventoryOpen;
	bool				m_bChestOpen;

	GameObject* m_pInventoryInface;
	GameObject* m_pChestIntreface;
	GameObject* m_pUICurSor;
	GameObject* m_pChestObject;

	Vec4				m_vOutInventoryPos;
	Vec4				m_vInInventorypos;

	Vec4				m_vOutChestPos;
	Vec4				m_vInChestPos;

	GameObject*			m_pInventorySlotObjs[20];
	GameObject*			m_pChestSlotObjs[28];

	int					m_vChestPos[2];
	int					m_vInventoryPos[2];

	vector<wstring>		m_vItemName;

	float				m_fSelectorSize;

	GameObject*			m_pSelectCircle;
	GameObject*			m_pSelectItemIcon;
	ITEM				m_iSelectItem;


	/**************************
	|	Interface Key
	**************************/
	GameObject* m_pInventoryInterface2;
	GameObject* m_pEquipSlotObjs[8];

	GameObject* m_pInfoText[4];
	GameObject* m_pObject_Village_Fade_BG;

	/**************************
	|	Pause State
	**************************/
	float				m_fPauseTime;

	/**************************
	|	Camera Move
	**************************/
	GameObject* m_pPlayerCam;
	Dir					m_eCameraMove;
	bool				m_bMoved;

	GameObject* m_pTopWall;
	GameObject* m_pBottomWall;
	GameObject* m_pRightWall;
	GameObject* m_pLeftWall;

	/*******************
	| Attack
	*******************/
	vector<ComboTask>	m_vComboTasks;
	GameObject* m_pSpearCollider;
	GameObject* m_pTwoHandCollider;

	float		m_fComboTime;
	int			m_iCurComboCount;
	int			m_iComboCount;

	/*******************
	| ESC
	*******************/
	int					m_iCurButton;
	vector<GameObject*> m_vButton;

	vector<GameObject*> m_vESCUI;
	GameObject* m_pSelectIcon1;
	GameObject* m_pSelectIcon2;

	/*******************
	| etc..
	*******************/
	GameObject* m_pMainLight;

	float		m_fSceneChangeTime;
	float		m_fDungeonEnterTime;

	TRIGGER_TYPE m_eSceneTrigger;
	bool		m_bSceneChange;

	/*******************
	| Dungeon Entrance
	*******************/
	GameObject* m_pDungeonPortal1;

public:
	virtual void Awake()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void LateUpdate() override {};

	virtual void OnTriggerEnter(Collider* _other);
	virtual void OnTriggerStay(Collider* _other);
	virtual void OnTriggerExit(Collider* _other);

	CLONE(PlayerScript)

public:
	PlayerPref* GetPlayerPref() { return m_pPlayerPref; }

	Dir GetDirection() { return m_eDir; }

private:
	void SetState(State _eState) { m_eState = _eState; }
	void SetDirection(Dir _eDir) { m_eDir = _eDir; }

private:
	void Invincibility();
	void Idle();
	void Attack();
	void Move();
	void Dodge();
	void Chest();
	void Inventory();
	void CameraMove();
	void Pause();
	void DungeonEnter();
	void SceneChange();
	void Scene();
	void ESC();
	void Dead();

	void IdleAnim();
	void AttackAnim();
	void MoveAnim();
	void DodgeAnim();
	void ChestAnim();
	void FallingAnim();
	void DeadAnim();

private:
	void ColliderPositionCalc();

public:
	PlayerScript();
	virtual ~PlayerScript();

	friend class TutorialScript;
};

