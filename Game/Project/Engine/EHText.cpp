#include "pch.h"
#include "EHText.h"

#include "EHEngine.h"
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

	_position.x *= -1.f;
	_position.y *= -1.f;

	_position.x += Engine::GetInst()->GetResolution().x / 2.f;
	_position.y += Engine::GetInst()->GetResolution().y / 2.f;

	UINT32 _color = 0;
	_color |= (UINT)m_Color.w;
	_color = _color << 8;
	_color |= (UINT)m_Color.z;
	_color = _color << 8;
	_color |= (UINT)m_Color.y;
	_color = _color << 8;
	_color |= (UINT)m_Color.x;

	FontMgr::GetInst()->DrawingText(m_sTextInput, _color, m_sFont, DWRITE_FONT_WEIGHT_DEMI_BOLD
		, _position.x - (m_sTextInput.size() * m_fFontSize) / 2.f
		, _position.y - m_fFontSize / 2.f, m_fFontSize);
}

void Text::Save(string _path)
{
	std::locale::global(std::locale(".UTF-8"));
	std::wofstream _file(_path, std::fstream::out | std::fstream::app);

	_file << "TEXT\n";

	_file << m_sTextInput + L'\n';

	_file << m_sFont + L'\n';

	wstring _weight = {};
	
	if (DWRITE_FONT_WEIGHT_BOLD == m_eWeight)
	{
		_weight = L"BOLD";
	}
	else if (DWRITE_FONT_WEIGHT_SEMI_BOLD == m_eWeight)
	{
		_weight = L"SEMI_BOLD";
	}
	else if (DWRITE_FONT_WEIGHT_MEDIUM == m_eWeight)
	{
		_weight = L"MEDIUM";
	}
	else if (DWRITE_FONT_WEIGHT_NORMAL == m_eWeight)
	{
		_weight = L"NORMAL";
	}
	else
	{
		_weight = L"THIN";
	}

	_file << _weight + L'\n';

	UINT32 _color = 0;
	_file << std::to_wstring(_color) + L'\n';

	_file << std::to_wstring(m_fFontSize) + L'\n';

	_file.close();
}
