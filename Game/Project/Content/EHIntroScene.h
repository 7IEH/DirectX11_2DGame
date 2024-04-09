#pragma once

#include "EHLevel.h"

class IntroLevel :
	public Level
{
private:
	float		       m_PlayTime;

public:
	virtual void Awake()override;
	virtual void Start() override;
	virtual void Update()override;
	virtual void FixedUpdate()override;
	virtual void LateUpdate()override;

public:
	IntroLevel();
	virtual ~IntroLevel();
};
