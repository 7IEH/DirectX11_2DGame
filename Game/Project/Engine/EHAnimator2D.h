#pragma once
#include "EHComponent.h"

#include "EHAsset.h"
#include "EHSprite.h"
class Animation2D;

class Animator2D :
	public Component
{
private:
	map<wstring, Animation2D*>			m_AnimInfo;
	Animation2D* m_CurAnimation;

	bool								m_Repeat;

public:
	Animation2D*			FindAnimation(const wstring& _strName);
	Animation2D*			CreateAnimation(const wstring& _animName, Ptr<Sprite> _atalas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS);

	void					Play(const wstring& _strName, bool _repeat = TRUE);
	static void				Clear();
public:
	virtual void			LateUpdate()override;
	virtual void			UpdateData()override;
public:
	Animator2D();
	virtual ~Animator2D();
};

