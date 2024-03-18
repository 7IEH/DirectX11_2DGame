#pragma once
#include <EHScript.h>
class TutorialScript :
	public Script
{
private:
	GameObject* m_pIntroObject;
	bool		m_bIntro;
	bool		m_bFirst;

public:
	virtual void Awake() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	CLONE(TutorialScript)

	TutorialScript();
	virtual ~TutorialScript();

};

