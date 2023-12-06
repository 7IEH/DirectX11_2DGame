#pragma once
class KeyMgr
	:public Singleton<KeyMgr>
{
	Single(KeyMgr);
public:
	void Init();
};

