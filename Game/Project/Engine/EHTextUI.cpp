#include "pch.h"
#include "EHTextUI.h"

#include "EHGameObject.h"
#include "EHText.h"

TextUI::TextUI()
	:ComponentUI("TextUI", "##TextUI", COMPONENT_TYPE::TEXT)
{
}

TextUI::~TextUI()
{
}

void TextUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Text* _text = GetTargetObject()->GetComponent<Text>(COMPONENT_TYPE::TEXT);
	assert(_text);
	ImGui::Text("Input Text");
	ImGui::Separator();
	wstring _wstr = _text->GetText();

	string _inputtext = string(_wstr.begin(), _wstr.end());
	ImGui::InputText("##TextInput", &_inputtext);
	_text->SetText(EH::ConvertWstring(_inputtext));

	string _font = EH::ConvertString(_text->GetFont());

	DWRITE_FONT_WEIGHT _weight = _text->GetFontWeight();
	string _sWeight = {};

	float _fontsize = _text->GetFontSize();

	float _fontcolor[4];
	switch (_weight)
	{
	case DWRITE_FONT_WEIGHT_BOLD:
	{
		_sWeight = "BOLD";
	}
	break;
	case DWRITE_FONT_WEIGHT_NORMAL:
	{
		_sWeight = "NORMAL";
	}
	break;
	case DWRITE_FONT_WEIGHT_THIN:
	{
		_sWeight = "THIN";

	}
	break;
	default:
		break;
	}

	if (ImGui::BeginChild("Main Settings"))
	{
		ImGui::Text("Font");ImGui::SameLine(100.f);
		ImGui::InputText("##TextInput", &_font);

		ImGui::Text("FontStyle");ImGui::SameLine(100.f);
		if (ImGui::BeginCombo("##FontStyle", _sWeight.c_str()))
		{
			if (ImGui::Selectable("BOLD", false))
			{
				_weight = DWRITE_FONT_WEIGHT_BOLD;
			}

			if (ImGui::Selectable("NORMAL", false))
			{
				_weight = DWRITE_FONT_WEIGHT_NORMAL;
			}

			if (ImGui::Selectable("THIN", false))
			{
				_weight = DWRITE_FONT_WEIGHT_THIN;
			}
			ImGui::EndCombo();
		}

		ImGui::Text("FontSize");ImGui::SameLine(100.f);

		ImGui::DragFloat("##FontSize", &_fontsize);


		ImGui::Text("FontColor");ImGui::SameLine(100.f);

		ImGui::ColorPicker4("FontColor", _fontcolor);

		ImGui::Text("Alignment");ImGui::SameLine(100.f);

		ImGui::EndChild();
	}

	_text->SetFontWeight(_weight);
	_text->SetFontSize(_fontsize);

	//TextInput();
	//MainSetting();
}

void TextUI::TextInput()
{
	Text* _text = GetTargetObject()->GetComponent<Text>(COMPONENT_TYPE::TEXT);
	assert(_text);
	//if (ImGui::BeginChild("Text Input"))
	//{
	//	////wstring _wstr = _text->GetText();
	//	////string _inputtext = EH::ConvertString(_wstr);
	//	//ImGui::InputText("##TextInput", &_inputtext);
	//	//_text->SetText(EH::ConvertWstring(_inputtext));

	//	ImGui::EndChild();
	//}
}

void TextUI::MainSetting()
{
	Text* _text = GetTargetObject()->GetComponent<Text>(COMPONENT_TYPE::TEXT);
	assert(_text);

	string _font = EH::ConvertString(_text->GetFont());

	DWRITE_FONT_WEIGHT _weight = _text->GetFontWeight();
	string _sWeight = {};

	float _fontsize = _text->GetFontSize();

	float _fontcolor[4];
	switch (_weight)
	{
	case DWRITE_FONT_WEIGHT_BOLD:
	{
		_sWeight = "BOLD";
	}
	break;
	case DWRITE_FONT_WEIGHT_NORMAL:
	{
		_sWeight = "NORMAL";
	}
	break;
	case DWRITE_FONT_WEIGHT_THIN:
	{
		_sWeight = "THIN";

	}
	break;
	default:
		break;
	}

	if (ImGui::BeginChild("Main Settings"))
	{
		ImGui::Text("Font");ImGui::SameLine(100.f);
		ImGui::InputText("##TextInput", &_font);

		ImGui::Text("FontStyle");ImGui::SameLine(100.f);
		if (ImGui::BeginCombo("##FontStyle", _sWeight.c_str()))
		{
			if (ImGui::Selectable("BOLD", false))
			{
				_weight = DWRITE_FONT_WEIGHT_BOLD;
			}

			if (ImGui::Selectable("NORMAL", false))
			{
				_weight = DWRITE_FONT_WEIGHT_NORMAL;
			}

			if (ImGui::Selectable("THIN", false))
			{
				_weight = DWRITE_FONT_WEIGHT_THIN;
			}
			ImGui::EndCombo();
		}

		ImGui::Text("FontSize");ImGui::SameLine(100.f);

		ImGui::DragFloat("##FontSize", &_fontsize);


		ImGui::Text("FontColor");ImGui::SameLine(100.f);

		ImGui::ColorPicker4("FontColor", _fontcolor);

		ImGui::Text("Alignment");ImGui::SameLine(100.f);

		ImGui::EndChild();
	}

	_text->SetFontWeight(_weight);
	_text->SetFontSize(_fontsize);
}