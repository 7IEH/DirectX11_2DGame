#pragma once
#include <EHScript.h>
class LoadingSceneScript :
	public Script
{
private:
	float m_fAccTime;
	GameObject* m_pLoadingUI;

public:
	virtual void Start()override;
	virtual void Update()override;

public:
	CLONE(LoadingSceneScript);
	LoadingSceneScript();
	virtual ~LoadingSceneScript();
};

