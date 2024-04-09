#pragma once
#include <EHScript.h>
class TutorialScript :
	public Script
{
private:
	GameObject*				m_pIntroObject;
	GameObject*				m_pIntroObjectBG;
	GameObject*				m_pFadeObject;
	GameObject*				m_pFadeObject2;
	GameObject*				m_pContinueButtonObject;
	GameObject*				m_pLoadingObject;
	GameObject*				m_pPlayer;
	GameObject*				m_pTutorialScroll;

	GameObject*				m_pUICamera;

	vector<GameObject*>		m_vTutorialBG;

	bool					m_bIntro;
	bool					m_bTutorialAwake;
	bool					m_bLoading;
	bool					m_bFirst;
	bool					m_bTrigger1;

	bool					m_bFirstScene;

	bool					m_bStart;

	float					m_fAcctime;

	bool					m_bSkip;

	/**************************
	|	Dungeon Object
	**************************/
	map<wstring, GameObject*> m_mDoorObjects;
	int						  m_iCurRoom;

	GameObject*			m_pButler;
	vector<GameObject*> m_vTurret;
	vector<GameObject*>	m_vSlime;
	GameObject*			m_pTangle;

public:
	virtual void Awake() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	void RoomOpen();

public:
	CLONE(TutorialScript)

	TutorialScript();
	virtual ~TutorialScript();
};

