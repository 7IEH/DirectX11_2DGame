#pragma once

class Animation2D;
#include "EHAnimation2D.h"

#include "EHAssetMgr.h"

class AnimationMgr
	:public Singleton<AnimationMgr>
{
	Single(AnimationMgr)
private:
	map<wstring, Animation2D*> m_AnimationInfo;

public:
	void Awake();
	void Save();

private:
	void SaveFrame(wstring _path, Animation2D* _anim);

public:
	Animation2D* PushAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, vector<tAnimFrameInfo>& _frameInfo);

	Animation2D* CreateAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS);

	Animation2D* EditAnimation2D(const wstring& _label, Ptr<Sprite> _atlas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS);

	void Insert(const wstring& _label, Animation2D* _anim);

	Animation2D* Find(const wstring& _label);

	map<wstring, Animation2D*>& GetAnimationInfo() { return m_AnimationInfo; }
};

