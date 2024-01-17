#pragma once

#include "EHScript.h"

class TriggerScript
	: public Script
{
private:
	TRIGGER_TYPE m_Type;
	
	/**************************
	|	Move Trigger member
	**************************/
	Vec4		m_PlayerPos;
	Vec4		m_CamPos;

public:
	void SetTriggerType(TRIGGER_TYPE _type) { m_Type = _type; }

	void SetPlayerPos(Vec4 _playerpos) { m_PlayerPos = _playerpos; }
	void SetCameraPos(Vec4 _campos) { m_CamPos = _campos; }

public:
	virtual void Awake() override;
	
	virtual void OnTriggerEnter(Collider* _other)override;

public:
	TriggerScript();
	virtual ~TriggerScript();
};

