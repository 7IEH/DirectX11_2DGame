#pragma once
#include "EHLevel.h"
class TitleLevel :
    public Level
{
private:
	GameObject* m_RightDoor;
	GameObject* m_LeftDoor;
	GameObject* m_StartLine;

	bool m_bOpen;

public:
	virtual void Awake()override;
	virtual void Start() override;
	virtual void Update()override;
	virtual void FixedUpdate()override;
	virtual void LateUpdate()override;

public:
	TitleLevel();
	virtual ~TitleLevel();
};

