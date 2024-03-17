#include "pch.h"
#include "EHSpriteLoader.h"

#include "EHPathMgr.h"
#include "EHImGUIMgr.h"
#include "EHAssetMgr.h"

SpriteLoader::SpriteLoader()
	:UI("SpriteLoader","##SpriteLoader")
{
}

SpriteLoader::~SpriteLoader()
{
}

void SpriteLoader::Render_Update()
{
	ImGui::Text("Name");
	ImGui::SameLine(100.f);
	ImGui::InputText("##SpriteName", &m_Name);

	ImGui::Text("Addresss");
	ImGui::SameLine(100.f);
	ImGui::InputText("##SpriteAddress", &m_Address);

	if (ImGui::Button("SaveSprite"))
	{
		wstring _path = PATH;
		_path += L"\\resource\\spritedata\\SpriteData.txt";

		FILE* _file = nullptr;
		fopen_s(&_file,string(_path.begin(), _path.end()).c_str(), "a");

		if (_file == nullptr)
			return;

		m_Name += "\n";
		fputs(m_Name.c_str(), _file);
		m_Address += "\n";
		fputs(m_Address.c_str(), _file);

		fclose(_file);

		AssetMgr::GetInst()->CreateSprite();

		m_Name = "";
		m_Address = "";
	}
}