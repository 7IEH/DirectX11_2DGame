#pragma once
#include <EHScript.h>
class TownScript :
	public Script
{
private:
	GameObject* m_pObject_Village_House5;
	GameObject* m_pObject_Village_Rival_Fountaion;
	GameObject* m_pObject_Village_Well;
	GameObject* m_pObject_Village_Will_Roof;
	GameObject* m_pObject_Village_BigTree1;
	GameObject* m_pObject_Village_BigTree2;
	GameObject* m_pObject_Village_BigTree3;
	GameObject* m_pObject_Village_Fade_BG;
	
	float	m_fTime;

public:
	virtual void Start()override;
	virtual void Update()override;

public:
	CLONE(TownScript);
	TownScript();
	virtual ~TownScript();
};

