#include "pch.h"
#include "EHAnimationMgr.h"

#include "EHAnimation2D.h"
#include "EHSprite.h"

AnimationMgr::AnimationMgr()
	:m_AnimationInfo{}
{

}

AnimationMgr::~AnimationMgr()
{

}

Animation2D* AnimationMgr::PushAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, vector<tAnimFrameInfo>& _frameInfo)
{
	Animation2D* _anim = Find(_label);

	if (_anim != nullptr)
	{
		return _anim;
	}

	_anim = new Animation2D;

	_anim->SetAnimationFrame(_label, _atlas, _frameInfo);
	m_AnimationInfo.insert(make_pair(_label, _anim));

	return _anim;
}

Animation2D* AnimationMgr::CreateAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, Vec2 _leftTop
	, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS)
{
	Animation2D* _anim = Find(_label);

	if (_anim != nullptr)
	{
		return _anim;
	}

	_anim = new Animation2D;

	_anim->Create(_label, _atlas, _leftTop, _offset, _sliceSize, _BackGround, _FrameCount, _FPS);
	m_AnimationInfo.insert(make_pair(_label, _anim));

	return _anim;
}

Animation2D* AnimationMgr::EditAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS)
{
	Animation2D* _anim = Find(_label);

	if (_anim != nullptr)
	{
		_anim->Edit(_label, _atlas, _leftTop, _offset, _sliceSize, _BackGround, _FrameCount, _FPS);
	}

	return _anim;
}

void AnimationMgr::Insert(const wstring& _label, Animation2D* _anim)
{
	Animation2D* _findAnim = Find(_label);

	if (_findAnim != nullptr)
		return;

	m_AnimationInfo.insert(make_pair(_label, _anim));
}

Animation2D* AnimationMgr::Find(const wstring& _label)
{
	map<wstring, Animation2D*>::iterator iter = m_AnimationInfo.find(_label);

	if (iter == m_AnimationInfo.end())
		return nullptr;

	return iter->second;
}
