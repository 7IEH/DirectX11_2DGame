#pragma once

#include "EHLevel.h"

#include <new>
#include <mfplay.h>
#pragma comment(lib,"mfplay.lib")

class IntroLevel :
	public Level
{
private:
	float		m_PlayTime;
	HWND		m_video;

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
