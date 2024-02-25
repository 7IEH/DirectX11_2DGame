#include "pch.h"
#include "EHText.h"

#include "EHDevice.h"
#include "EHFontMgr.h"

#include "EHGameObject.h"

Text::Text()
	:Component(COMPONENT_TYPE::TEXT)
	, m_Color(0xffffffff)
	, m_sFont(L"Arial")
	, m_eWeight(DWRITE_FONT_WEIGHT_NORMAL)
	, m_fFontSize(32.f)
{
}

Text::~Text()
{
}

void Text::Update()
{
}

void Text::Render()
{
	Vec4 _position = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition();
	FontMgr::GetInst()->DrawingText(m_sTextInput, m_Color, m_sFont, m_eWeight, _position.x, _position.y, m_fFontSize);
}

void Text::Save(string _path)
{
	std::ofstream _file(_path, std::fstream::out | std::fstream::app);

	_file << "TEXT\n";

	_file << EH::ConvertString(m_sTextInput.c_str()) + '\n';

	_file << EH::ConvertString(m_sFont.c_str()) + '\n';

	wstring _weight = {};

	if (DWRITE_FONT_WEIGHT_BOLD == m_eWeight)
	{
		_weight = L"BOLD";
	}
	else if (DWRITE_FONT_WEIGHT_NORMAL == m_eWeight)
	{
		_weight = L"NORMAL";
	}
	else
	{
		_weight = L"THIN";
	}

	_file << EH::ConvertString(_weight.c_str()) + '\n';

	_file << std::to_string(m_Color) + '\n';

	_file << std::to_string(m_fFontSize) + '\n';

	_file.close();
}
