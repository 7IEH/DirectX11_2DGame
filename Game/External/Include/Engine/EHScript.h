#pragma once

#include "EHComponent.h"

class Script
	:public Component
{
private:
	UINT		m_iScriptType;

public:
	virtual void Awake() override {};
	virtual void Start()override {};
	virtual void Update() override {};
	virtual void FixedUpdate() override {};
	virtual void LateUpdate() override {};

public:
	Script();
	virtual ~Script();
};

