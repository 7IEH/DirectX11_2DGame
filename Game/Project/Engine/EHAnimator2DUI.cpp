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

	// �ִϸ����� ������Ʈ ������
	Animator2D* _animator = GetTargetObject()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
	
	// ���� �ִϸ��̼� ���� ������
	// string ��ȯ���� Imgui ��

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

	// ���� Animation �������� �ҷ���
	map<wstring, Animation2D*> _animInfo = AnimationMgr::GetInst()->GetAnimationInfo();
	map<wstring, Animation2D*>::iterator iter = _animInfo.begin();

	// Combo ������ ���õǸ� �ش� selectable ���� ����
	// ��, ���� �ִϸ��̼��� �̸��� �ٲ�
	// �������� ���� �ִϸ��̼��� �ٲ��ָ� ��
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

	// ���� �ִϸ��̼� ������
	Animation2D* _curAnimation = _animator->GetCurAnimation2D();

	if (nullptr != _curAnimation)
	{
		// ���� �ִϸ��̼��� atlas ���� ������
		Ptr<Sprite> _curAtlas = _curAnimation->GetAtlas();

		float _texWidth = float(_curAtlas.Get()->GetSpriteWidth());
		float _texHeight = float(_curAtlas.Get()->GetSpriteHeight());

		// ���� �ִϸ��̼� ������ ����
		UINT _FrameCount = _curAnimation->GetCurFrame();
		vector<tAnimFrameInfo>& _FrameInfo = _curAnimation->GetFrameInfo();

		ImVec2 _stUV = ImVec2(0.f, 0.f);
		ImVec2 _enUV = ImVec2(0.f, 0.f);

		// �ִϸ��̼� ��� ���� Preview
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

		// SameLine ����
		ImGui::Text("");
	}
}