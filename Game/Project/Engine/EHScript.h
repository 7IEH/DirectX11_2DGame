#pragma once

#include "EHComponent.h"

class Collider;

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
	virtual void OnTriggerEnter(Collider* _other) {};
	virtual void OnTriggerStay(Collider* _other) {};
	virtual void OnTriggerExit(Collider* _other) {};

public:
	Script();
	virtual ~Script();
};

