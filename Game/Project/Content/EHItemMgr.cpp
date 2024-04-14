#include "pch.h"
#include "EHItemMgr.h"

ItemMgr::ItemMgr()
	: m_sItemMat{}
{
}

ItemMgr::~ItemMgr()
{
}

void ItemMgr::Awake()
{
	m_sItemMat[1] = L"HUD_Potion_Icon_Mat";
	m_sItemMat[2] = L"HUD_BigSword_Icon_Mat";
	m_sItemMat[3] = L"HUD_BroomSpear_Icon_Mat";
}