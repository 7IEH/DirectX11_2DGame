#include "pch.h"
#include "EHSpriteEditor.h"

#include "EHAssetMgr.h"

#include "EHAnimationMgr.h"
#include "EHAnimation2D.h"

#include "EHImGUIMgr.h"

#include "EHAnimationUI.h"

SpriteEditor::SpriteEditor()
	:UI("SpriteEditor", "##SpriteEditor")
{
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Render_Update()
{
	map<wstring, Animation2D*> _animInfo = AnimationMgr::GetInst()->GetAnimationInfo();

	map<wstring, Animation2D*>::iterator iter = _animInfo.begin();

	if (ImGui::Button("SortingAtlas##AnimationUI", ImVec2(200.f, 30.f)))
	{
		ImGUIMgr::GetInst()->SetAnimationUI(TRUE);
		AnimationUI* _ui = dynamic_cast<AnimationUI*>(ImGUIMgr::GetInst()->GetAnimationUI());
		_ui->SetType(ANIMATION_UI::CREATE);
	}
	ImGui::SameLine();

	if (ImGui::Button("UnSortingAtlas##AnimationUIunsorting", ImVec2(200.f, 30.f)))
	{
		ImGUIMgr::GetInst()->SetunAnimationCreateUI(TRUE);
	}

	ImGui::SameLine();
	if (ImGui::Button("Load##LoadSpriteImage", ImVec2(80.f, 30.f)))
	{
		ImGUIMgr::GetInst()->SetSpriteLoader(TRUE);
	}

	ImGui::SameLine();
	if (ImGui::Button("Exit##ExitSpriteImage",ImVec2(80.f, 30.f)))
	{
		ImGUIMgr::GetInst()->SetSprite(FALSE);
	}

	ImVec2 _winsize = ImGui::GetWindowSize();
	ImGui::BeginListBox("##List", _winsize);

	bool flags = false;
	int _idx = 0;
	for (;iter != _animInfo.end();iter++)
	{
		string _name = string(iter->first.begin(), iter->first.end());
		string _label = "AnimationData" + std::to_string(_idx);

		if (ImGui::Selectable(_label.c_str(), &flags))
		{
			ImGUIMgr::GetInst()->SetAnimationUI(TRUE);
			AnimationUI* _ui = dynamic_cast<AnimationUI*>(ImGUIMgr::GetInst()->GetAnimationUI());
			_ui->SetType(ANIMATION_UI::EDIT);

			Animation2D* _anim = AnimationMgr::GetInst()->Find(wstring(_name.begin(), _name.end()));
			_ui->SetLabel(_name);
			_ui->SetTexName(string(_anim->GetAtlas()->GetName().begin(), _anim->GetAtlas()->GetName().end()));
		}
		ImGui::Text("Name : ");
		ImGui::SameLine();
		ImGui::Text(_name.c_str());
		ImGui::Text("SpriteSheet");
		ImGui::Separator();
		float _width = iter->second->GetAtlas().Get()->GetSpriteWidth();
		float _height = iter->second->GetAtlas().Get()->GetSpriteHeight();
		ImGui::Image(iter->second->GetAtlas().Get()->GetSRV().Get(),ImVec2(_width,_height));
		ImGui::Separator();
		_idx++;
	}

	ImGui::EndListBox();
}