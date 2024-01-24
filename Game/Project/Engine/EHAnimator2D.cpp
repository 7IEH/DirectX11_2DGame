#include "pch.h"
#include "EHAnimator2D.h"

#include "EHAnimation2D.h"

#include "EHAnimationMgr.h"

Animator2D::Animator2D()
	:Component(COMPONENT_TYPE::ANIMATOR2D)
	, m_AnimInfo{}
	, m_CurAnimation(nullptr)
	, m_Repeat(FALSE)
{
}

Animator2D::~Animator2D()
{
	map<wstring, Animation2D*>::iterator iter = m_AnimInfo.begin();

	for (;iter != m_AnimInfo.end();iter++)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}

}

Animation2D* Animator2D::FindAnimation(const wstring& _strName)
{
	map<wstring, Animation2D*>::iterator iter = m_AnimInfo.find(_strName);

	if (iter == m_AnimInfo.end())
	{
		return nullptr;
	}

	return (*iter).second;
}

Animation2D* Animator2D::CreateAnimation(const wstring& _animName, Ptr<Sprite> _atalas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize, Vec2 _BackGround, UINT _FrameCount, float _FPS)
{
	Animation2D* _anim = FindAnimation(_animName);
	if (nullptr != _anim)
	{
		return _anim;
	}

	_anim = new Animation2D;

	_anim->Create(_animName, _atalas, _leftTop, _offset, _sliceSize, _BackGround, _FrameCount, _FPS);
	m_AnimInfo.insert(make_pair(_animName, _anim));

	AnimationMgr::GetInst()->Insert(_animName, _anim);

	return _anim;
}

void Animator2D::Play(const wstring& _strName, bool _repeat)
{
	Animation2D* _anim = FindAnimation(_strName);
	if (_anim == nullptr)
	{
		_anim = AnimationMgr::GetInst()->Find(_strName);
		if (_anim == nullptr)
			return;
	}

	m_Repeat = _repeat;
	m_CurAnimation = _anim;
	m_CurAnimation->Reset();
}


void Animator2D::UpdateData()
{
	if (m_CurAnimation == nullptr)
		return;

	m_CurAnimation->UpdateData();
}

void Animator2D::LateUpdate()
{
	if (m_CurAnimation == nullptr)
		return;

	if (m_CurAnimation->IsFinish() && m_Repeat)
	{
		m_CurAnimation->Reset();
	}

	m_CurAnimation->LateUpdate();
}

void Animator2D::Clear()
{
	Animation2D::Clear();
}