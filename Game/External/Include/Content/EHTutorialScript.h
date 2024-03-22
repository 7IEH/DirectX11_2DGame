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

	vector<GameObject*>		m_vTutorialBG;

	bool					m_bIntro;
	bool					m_bTutorialAwake;
	bool					m_bLoading;
	bool					m_bFirst;
	bool					m_bTrigger1;

	float					m_fAcctime;
	
	bool					m_bSkip;

public:
	virtual void Awake() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	CLONE(TutorialScript)

	TutorialScript();
	virtual ~TutorialScript();

};

