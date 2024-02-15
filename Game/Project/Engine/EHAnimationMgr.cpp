#include "pch.h"
#include "EHAnimationMgr.h"

#include "EHAnimation2D.h"
#include "EHSprite.h"

#include "EHPathMgr.h"

AnimationMgr::AnimationMgr()
	:m_AnimationInfo{}
{

}

AnimationMgr::~AnimationMgr()
{
	ReleaseMap<wstring,Animation2D*>(m_AnimationInfo);
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

void AnimationMgr::Awake()
{
	// Animation 정보 Load
	string _absoulte = EH::ConvertString(PATH);
	string _path = _absoulte + "\\Assets\\Animator2DData";

	for (const std::filesystem::directory_entry& _entry :
		std::filesystem::directory_iterator(_path))
	{
		wstring _aniName; _aniName.clear();
		Ptr<Sprite> _atlas = nullptr;
		vector<tAnimFrameInfo> _frameInfo; _frameInfo.clear();
		Vec2 _leftTop = Vec2(0.f);
		Vec2 _offset = Vec2(0.f);
		Vec2 _sliceSize = Vec2(0.f);
		Vec2 _backGround = Vec2(0.f);
		UINT _frameCount = 0;
		float _FPS = 0.f;

		wstring _fileName = _entry.path().native().substr(_entry.path().native().find_last_of('\\') + 1);
		wstring _extention = _fileName.substr(_fileName.find_last_of('.') + 1);

		if (_extention == L"anim")
		{
			std::ifstream _file(_entry.path().native().data());
			string _line = "";
			_aniName = _fileName.substr(0, _fileName.find_last_of('.'));

			std::getline(_file, _line);
			_frameCount = std::stoi(_line);
			_line.clear();

			std::getline(_file, _line);
			_atlas = AssetMgr::GetInst()->FindAsset<Sprite>(EH::ConvertWstring(_line));

			_frameInfo.resize(_frameCount);

			for (UINT i = 0;i < _frameCount;i++)
			{
				std::getline(_file, _line);
				EH::InputVector2(_line, _frameInfo[i]._LeftTop);
				_line.clear();

				std::getline(_file, _line);
				EH::InputVector2(_line, _frameInfo[i]._Offset);
				_line.clear();

				std::getline(_file, _line);
				EH::InputVector2(_line, _frameInfo[i]._SliceSize);
				_line.clear();

				std::getline(_file, _line);
				EH::InputVector2(_line, _frameInfo[i]._BackGround);
				_line.clear();

				std::getline(_file, _line);
				_frameInfo[i]._Duration = std::stof(_line);
				_line.clear();
			}

			PushAnimation2D(_aniName, _atlas, _frameInfo);
			_file.close();
		}
	}
}

void AnimationMgr::Save()
{
	// Animation 정보 Save
	string _absoulte = EH::ConvertString(PATH);
	string _path = _absoulte + "\\Assets\\Animator2DData";

	map<wstring, Animation2D*>::iterator iter = m_AnimationInfo.begin();

	for (;iter != m_AnimationInfo.end();iter++)
	{
		bool _flag = false;
		for (const std::filesystem::directory_entry& _entry :
			std::filesystem::directory_iterator(_path))
		{
			wstring _fileName = _entry.path().native().substr(_entry.path().native().find_last_of('\\') + 1);
			wstring _extention = _fileName.substr(_fileName.find_last_of('.') + 1);

			if (_extention == L"anim")
			{
				wstring _aniName = _fileName.substr(0, _fileName.find_last_of('.'));

				if (iter->first == _aniName)
				{
					_flag = true;
					// 저장된 Animation 정보가 있을 경우
					std::ofstream _file(_entry.path().native().data());
					_file.clear();
					_file.close();

					SaveFrame(_entry.path().native(), iter->second);
				}
			}
		}

		if (!_flag)
		{
			// 저장된 Animation 정보가 없을 경우
			// _atlas _frameCount wstring _aniName; _aniName.clear();
			string _animPath = _path + "\\" + EH::ConvertString(iter->first) + ".anim";

			SaveFrame(EH::ConvertWstring(_animPath), iter->second);
		}
	}
}

void AnimationMgr::SaveFrame(wstring _path, Animation2D* _anim)
{
	std::ofstream _file(_path.data());
	vector<tAnimFrameInfo> _frameInfo = _anim->GetFrameInfo();
	UINT _frameCount = UINT(_frameInfo.size());
	string _atlasName = EH::ConvertString(_anim->GetAtlas()->GetName());

	_file << std::to_string(_frameCount) + '\n';
	_file << _atlasName + '\n';

	for (size_t i = 0;i < _frameCount;i++)
	{
		Vec2 _leftTop = _frameInfo[i]._LeftTop;
		Vec2 _offset = _frameInfo[i]._Offset;
		Vec2 _sliceSize = _frameInfo[i]._SliceSize;
		Vec2 _backGround = _frameInfo[i]._BackGround;
		float _FPS = _frameInfo[i]._Duration;

		_file << EH::WriteVector2(_leftTop) + '\n';
		_file << EH::WriteVector2(_offset) + '\n';
		_file << EH::WriteVector2(_sliceSize) + '\n';
		_file << EH::WriteVector2(_backGround) + '\n';
		_file << std::to_string(_FPS) + '\n';
	}
	_file.close();
}