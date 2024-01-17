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
	ROOM_TYPE	m_Room;
	SPAWN_TYPE	m_Dir;


public:
	virtual void Awake() override;
	
	virtual void OnTriggerEnter(Collider* _other)override;

public:
	TriggerScript();
	virtual ~TriggerScript();
};

