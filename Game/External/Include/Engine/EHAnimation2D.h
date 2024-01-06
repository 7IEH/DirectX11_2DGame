#pragma once

#include "EHEntity.h"

struct tAnimFrameInfo
{
	Vec2    _LeftTop;
	Vec2    _SliceSize;
	Vec2    _Offset;
	Vec2	_BackGround;
	float   _Duration;
};
#include "EHSprite.h"

class Animator2D;
class Animation2D
	:public Entity
{
private:
	vector<tAnimFrameInfo>		m_FrameInfo;
	int							m_CurFrame;

	Animator2D*					m_Animator;
	Ptr<Sprite>					m_AtlasSprite;

	float						m_PlayTime;
	bool						m_Finsih;

public:
	void SetAnimator(Animator2D* _animator) { m_Animator = _animator; }
	Animator2D* GetAnimator() { return m_Animator; }

public:
	void UpdateData();
	void LateUpdate();
	static void Clear();

private:
	void Create(const wstring& _animName, Ptr<Sprite> _atalas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS);

	bool GetFinish() { return m_Finsih; }
	void SetFinish(bool _finish) { m_Finsih = _finish; }

public:
	Animation2D();
	~Animation2D();

	friend class Animator2D;
};

