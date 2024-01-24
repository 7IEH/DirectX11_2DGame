#pragma once

#include "EHUI.h"

#include "EHSprite.h"

class AnimationUI
	:public UI
{
private:
	ANIMATION_UI m_Type;

	string m_CurrentLabel;
	string m_PrevLabel;

	float m_LeftTop[2];
	float m_SliceSize[2];
	int m_frameCount;
	float m_fps;
	string m_Name;

	// Preview
	float m_PreViewWidth;
	bool m_Play;

	// Sprite Preview
	float m_Acctime;
	float m_SpriteWidth;

public:
	void SetType(ANIMATION_UI _type) { m_Type = _type; }

	void SetLabel(string _currentLabel) { m_Name = _currentLabel; }
	void SetTexName(string _currentLabel) { m_CurrentLabel = _currentLabel; }

public:
	virtual void Render_Update() override;

private:
	void Cavas(Ptr<Sprite> _AtlasImage);

public:
	AnimationUI();
	virtual ~AnimationUI();
};

