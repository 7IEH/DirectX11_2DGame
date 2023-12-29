#pragma once

#include "EHLevel.h"

class TestLevel
	:public Level
{
private:

public:
	virtual void Awake()override;
	virtual void Start() override;
	virtual void Update()override;
	virtual void FixedUpdate()override;
	virtual void LateUpdate()override;

public:
	TestLevel();
	virtual ~TestLevel();
};

