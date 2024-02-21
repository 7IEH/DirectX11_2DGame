#include "pch.h"
#include "EHAnimator2DUI.h"

#include "EHGameObject.h"

#include "EHAnimator2D.h"
#include "EHAnimation2D.h"

#include "EHAnimationMgr.h"

Animator2DUI::Animator2DUI()
	:ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
	,m_prevAnimation2D(" ")
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	/******************************
	|	1. Select Play Animation
	******************************/

	// 애니메이터 컴포넌트 가져옴
	Animator2D* _animator = GetTargetObject()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
	
	// 현재 애니메이션 정보 가져옴
	// string 변환까지 Imgui 용

	wstring _wcurAnimation2D = L" ";
	if (nullptr == _animator->GetCurAnimation2D())
	{
		_wcurAnimation2D = L" ";
	}
	else
	{
		_wcurAnimation2D = _animator->GetCurAnimation2D()->GetName();
	}
	string _curAnimation2D = string(_wcurAnimation2D.begin(), _wcurAnimation2D.end());

	// 현재 Animation 정보들을 불러옴
	map<wstring, Animation2D*> _animInfo = AnimationMgr::GetInst()->GetAnimationInfo();
	map<wstring, Animation2D*>::iterator iter = _animInfo.begin();

	// Combo 원리는 선택되면 해당 selectable 문을 충족
	// 즉, 현재 애니메이션의 이름이 바뀜
	// 다음으로 현재 애니메이션을 바꿔주면 됨
	ImGui::Text("Animation");ImGui::SameLine();
	bool _flag = false;
	if (ImGui::BeginCombo("##PlayAnimation2D", _curAnimation2D.c_str()))
	{
		for (;iter != _animInfo.end();iter++)
		{
			bool is_selected = (_wcurAnimation2D == iter->first);
			string _comboChild = string(iter->first.begin(), iter->first.end());
			if (ImGui::Selectable(_comboChild.c_str(), &_flag))
			{
				_curAnimation2D = _comboChild.c_str();
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	_wcurAnimation2D = wstring(_curAnimation2D.begin(), _curAnimation2D.end());

	if (m_prevAnimation2D != _curAnimation2D)
	{
		if (" " != _curAnimation2D)
		{
			_animator->AddAnimation2D(_wcurAnimation2D);
			_animator->Play(_wcurAnimation2D, TRUE);
			m_prevAnimation2D = _curAnimation2D;
		}
	}

	/*************************
	|	2. Atlas PreView
	*************************/
	ImGui::Text("Atlas Texture");
	ImGui::Separator();

	// 현재 애니메이션 가져옴
	Animation2D* _curAnimation = _animator->GetCurAnimation2D();

	if (nullptr != _curAnimation)
	{
		// 현재 애니메이션의 atlas 정보 가져옴
		Ptr<Sprite> _curAtlas = _curAnimation->GetAtlas();

		float _texWidth = float(_curAtlas.Get()->GetSpriteWidth());
		float _texHeight = float(_curAtlas.Get()->GetSpriteHeight());

		// 현재 애니메이션 프레임 정보
		UINT _FrameCount = _curAnimation->GetCurFrame();
		vector<tAnimFrameInfo>& _FrameInfo = _curAnimation->GetFrameInfo();

		ImVec2 _stUV = ImVec2(0.f, 0.f);
		ImVec2 _enUV = ImVec2(0.f, 0.f);

		// 애니메이션 재생 정보 Preview
		for (size_t _frame = 0; _frame < _FrameInfo.size();_frame++)
		{
			_stUV = ImVec2(_FrameInfo[_frame]._LeftTop.x, _FrameInfo[_frame]._LeftTop.y);
			_enUV = ImVec2(_FrameInfo[_frame]._LeftTop.x + _FrameInfo[_frame]._SliceSize.x, _FrameInfo[_frame]._LeftTop.y + _FrameInfo[_frame]._SliceSize.y);

			if (_frame == _FrameCount)
			{
				ImGui::ImageButton(_curAtlas.Get()->GetSRV().Get(), ImVec2(20.f, 20.f), _stUV, _enUV, -1, ImVec4(0, 0, 0, 0), ImVec4(1.f, 0.f, 0.f, 1.f));
			}
			else
			{
				ImGui::ImageButton(_curAtlas.Get()->GetSRV().Get(), ImVec2(20.f, 20.f), _stUV, _enUV);
			}
			ImGui::SameLine();
		}

		// SameLine 방지
		ImGui::Text("");
	}
}