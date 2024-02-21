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

Animator2D::Animator2D(const Animator2D& _origin)
	:Component(_origin)
	, m_AnimInfo{}
	, m_CurAnimation(nullptr)
	, m_Repeat()
{
	map<wstring, Animation2D*>::const_iterator iter = _origin.m_AnimInfo.begin();

	for (;iter != _origin.m_AnimInfo.end();iter++)
	{
		Animation2D* _copy = iter->second->Clone();

		_copy->SetAnimator(this);
		m_AnimInfo.insert(make_pair(iter->first, _copy));
	}

	if (nullptr != _origin.m_CurAnimation)
	{
		m_CurAnimation = FindAnimation(_origin.m_CurAnimation->GetName());
	}
}

Animator2D::~Animator2D()
{
	ReleaseMap(m_AnimInfo);
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

HRESULT Animator2D::AddAnimation2D(const wstring& _strName)
{
	Animation2D* _anim = nullptr;

	_anim = FindAnimation(_strName);

	if (nullptr != _anim)
		return S_OK;

	_anim = AnimationMgr::GetInst()->Find(_strName);
	_anim = _anim->Clone();

	if (_anim == nullptr)
	{
		HandleError(MAIN_HWND, _strName + L" Animation2D Loading Failure!", 0);
		return E_FAIL;
	}

	m_AnimInfo.insert(make_pair(_strName, _anim));

	return S_OK;
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

void Animator2D::Save(string _path)
{
	// 1. Animation Size 2. Animation Name 3. 현재 애니메이션
	std::ofstream _file(_path.data(), std::fstream::out | std::fstream::app);

	size_t _size = m_AnimInfo.size();
	map<wstring, Animation2D*>::iterator iter = m_AnimInfo.begin();

	_file << "ANIMATOR2D\n";
	_file << std::to_string(_size) + '\n';
	for (;iter != m_AnimInfo.end();iter++)
	{
		_file << EH::ConvertString(iter->first) + '\n';
	}

	_file.close();
}