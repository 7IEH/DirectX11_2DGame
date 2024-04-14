#pragma once

#include "EHScript.h"
#include "EHItemScript.h"

class TriggerScript
	: public Script
{
private:
	TRIGGER_TYPE m_eTriggerType;

	/**************************
	|	Chset Trigger member
	**************************/
	ITEM	     m_eChestStore[28];
	int		     m_iChestStore[28];
	int			 m_iChestNum;

	/**************************
	|	Move Trigger member
	**************************/
	Vec4		m_vPlayerPos;
	Vec4		m_vCamPos;

	GameObject* m_pCam;

public:
	void SetTriggerType(TRIGGER_TYPE _eType) { m_eTriggerType = _eType; }
	TRIGGER_TYPE GetTriggerType() { return m_eTriggerType; }

	void SetPlayerPos(Vec4 _vPlayerpos) { m_vPlayerPos = _vPlayerpos; }
	void SetCameraPos(Vec4 _vCampos) { m_vCamPos = _vCampos; }

	void SetChestNum(int _iChestNum) { m_iChestNum = _iChestNum; }

	void SetChestItem(int _iChestItem, int _itemCount, int _x, int _y) {
		m_eChestStore[(_y * 7) + _x] = ITEM(_iChestItem);
		m_iChestStore[(_y * 7) + _x] = _itemCount;
	}

	ITEM* GetChestItem() { return m_eChestStore; }

	CLONE(TriggerScript)

public:
	virtual void Awake() override;

public:
	TriggerScript();
	virtual ~TriggerScript();
};

