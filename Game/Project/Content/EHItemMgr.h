#pragma once

class ItemMgr
	:public Singleton<ItemMgr>
{
	Single(ItemMgr)

private:
	wstring m_sItemMat[4];

public:
	wstring GetMaterial(int _idx) { return m_sItemMat[_idx]; }

public:
	void Awake();
};

