#include "pch.h"
#include "EHAnimation2D.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHSprite.h"

#include "EHTimeMgr.h"

Animation2D::Animation2D()
	: m_FrameInfo{}
	, m_Animator(nullptr)
	, m_CurFrame(0)
	, m_PlayTime(0.f)
	, m_Finsih(false)
{
}

Animation2D::~Animation2D()
{
}

void Animation2D::Create(const wstring& _animName, Ptr<Sprite> _atalas, Vec2 _leftTop, Vec2 _offset, Vec2 _sliceSize,Vec2 _BackGround, UINT _FrameCount, float _FPS)
{
	SetName(_animName);

	tAnimFrameInfo _info = {};

	m_AtlasSprite = _atalas;

	UINT _width = m_AtlasSprite->GetSpriteWidth();
	UINT _height = m_AtlasSprite->GetSpriteHeight();

	for (UINT i = 0;i < _FrameCount;i++)
	{
		_info._Duration = 1.f/ _FPS;
		_info._SliceSize = Vec2(_sliceSize.x / (float)_width, _sliceSize.y / (float)_height);
		_info._LeftTop = Vec2(_leftTop.x / (float)_width + _info._SliceSize.x * i, _leftTop.y / (float)_height);
		_info._Offset = Vec2(_offset.x / (float)_width, _offset.y / (float)_height);
		_info._BackGround = Vec2(_BackGround.x / (float)_atalas->GetSpriteWidth(), _BackGround.y / (float)_atalas->GetSpriteHeight());
		m_FrameInfo.push_back(_info);
	}
}

void Animation2D::UpdateData()
{
	if (m_AtlasSprite.Get() != nullptr)
		m_AtlasSprite->UpdateData(10);

	tAnimationInfo _animationInfo = {};
	_animationInfo._AnimUse = 1;
	_animationInfo._LeftTop = m_FrameInfo[m_CurFrame]._LeftTop;
	_animationInfo._OffsetSize = m_FrameInfo[m_CurFrame]._Offset;
	_animationInfo._SliceSize = m_FrameInfo[m_CurFrame]._SliceSize;
	_animationInfo._BackGround = m_FrameInfo[m_CurFrame]._BackGround;

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::ANIMATION)->SetData(&_animationInfo, sizeof(tAnimationInfo), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::ANIMATION)->UpdateData();
}

void Animation2D::LateUpdate()
{
	m_PlayTime += DT;
	if (m_FrameInfo[m_CurFrame]._Duration <= m_PlayTime)
	{
		++m_CurFrame;
		if (m_FrameInfo.size() <= m_CurFrame)
		{
			m_CurFrame = 0;
			//m_Finsih = TRUE;
		}
		m_PlayTime = 0.f;
	}
}

void Animation2D::Clear()
{
	tAnimationInfo _animationInfo = {};
	_animationInfo._AnimUse = 0;

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::ANIMATION)->SetData(&_animationInfo, sizeof(tAnimationInfo), 1);
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::ANIMATION)->UpdateData();
}
