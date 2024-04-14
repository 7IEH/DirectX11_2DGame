#pragma once
#include "EHScript.h"

enum class ITEM
{
	NO_ITEM,
	PORTION,
	DOUBLE_HAND_SWORD,
	BROOM_STICK
};

class ItemScript :
	public Script
{
private:
	ITEM m_eItemSerial;
	
public:
	virtual void Use() {};
	virtual void SetItemNumber(ITEM _eItemSerial) { m_eItemSerial = _eItemSerial; }

public:
	CLONE(ItemScript);

	ItemScript();
	virtual ~ItemScript();
};

