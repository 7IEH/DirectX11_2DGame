#pragma once
#include <EHScript.h>

class ProjecTileScript :
	public Script
{
private:
	Dir     m_eDir;
	float   m_fSpeed;
	int     m_iDamage;

	wstring m_sDeadAnimName;

public:
	void SetDir(Dir _eDir) { m_eDir = _eDir; }
	void SetSpeed(float _fSPeed) { m_fSpeed = _fSPeed; }
	void SetDamage(int _iDamage) { m_iDamage = _iDamage; }

	void SetDeadAnimName(const wstring& _sDeadAnimName) { m_sDeadAnimName = _sDeadAnimName; }

	int GetDamage() { return m_iDamage; }

public:
	virtual void Update()override;

	virtual void OnTriggerEnter(Collider* _other) override;
public:
	CLONE(ProjecTileScript)
	ProjecTileScript();
	virtual ~ProjecTileScript();
};

