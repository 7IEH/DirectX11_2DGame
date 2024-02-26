#pragma once
#include "EHScript.h"
class TitleScript :
	public Script
{
private:
	bool m_bOpen;
	bool m_bStart;
	bool m_bFirst;
	bool m_bSecond;
	bool m_bThird;

	GameObject* m_PointLight1;
	GameObject* m_PointLight2;

	GameObject* m_StartLine;

	GameObject* m_RightDoor;
	GameObject* m_LeftDoor;
	GameObject* m_GameLogo;

	GameObject* m_StartButton;
	GameObject* m_OptionButton;
	GameObject* m_ExitButton;

	GameObject* m_TitleBG1;
	GameObject* m_TitleBG2;

	float m_fOpenTime;
	float m_fAcctime;
	float m_fAcctime2;
	float m_fAcctime3;

	float m_fSpeed;
	float m_fSpeed2;

	float m_fFrequency;
	float m_fAmplitude;

	float m_fShake;

	float m_Radius1;
	float m_Radius2;

public:
	void StartGame();
	void LoadGame();
	void Option();

public:
	virtual void Start();

	virtual void Update();
	virtual void LateUpdate();

	CLONE(TitleScript)

private:
	Vec4 DoorMove(Vec4 _src, Vec4 _dest, float _speed, bool _flag);

public:
	TitleScript();
	virtual ~TitleScript();

};
