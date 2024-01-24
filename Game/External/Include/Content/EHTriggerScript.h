#pragma once

#include "EHScript.h"

class TriggerScript
	: public Script
{
private:
	TRIGGER_TYPE m_TriggerType;
	
	/**************************
	|	Move Trigger member
	**************************/
	Vec4		m_PlayerPos;
	Vec4		m_CamPos;

	GameObject* m_Cam;

public:
	void SetTriggerType(TRIGGER_TYPE _type) { m_TriggerType = _type; }

	void SetPlayerPos(Vec4 _playerpos) { m_PlayerPos = _playerpos; }
	void SetCameraPos(Vec4 _campos) { m_CamPos = _campos; }

public:
	virtual void Awake() override;
	
	virtual void OnTriggerEnter(Collider* _other)override;

public:
	TriggerScript();
	virtual ~TriggerScript();
};

