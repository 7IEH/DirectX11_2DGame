#include "pch.h"
#include "EHMaterialCreator.h"
#include "EHAssetMgr.h"
#include "EHPathMgr.h"

MaterialCreator::MaterialCreator()
	:UI("MaterialCreator", "##MaterialCreator")
{
}

MaterialCreator::~MaterialCreator()
{
}

void MaterialCreator::Render_Update()
{
	string _name = {};
	string _shader = {};
	string _sprite = {};

	wstring _wName = {};
	wstring _wShader = {};
	wstring _wSprite = {};

	if (ImGui::Button("Create"))
	{
		wstring _wrealtivePath = PATH;
		wstring _wPath = _wrealtivePath + L"\\resource\\Material\\materialdata\\Material.txt";
		std::wofstream _pFile(_wPath.c_str(), std::ios::app);

		_pFile << _wName;
		_pFile << _wShader;
		_pFile << _wSprite;

		// assetmgr 실행 시 material 만들기도 바꿈
		Material* _mat = new Material;
		_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(EH::ConvertWstring(_shader).c_str()));
		_mat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(EH::ConvertWstring(_sprite).c_str()));
		AssetMgr::GetInst()->AddAsset<Material>(_mat, EH::ConvertWstring(_name).c_str());
	}

	if (ImGui::Button("Exit"))
	{
		// 나가기
	}

	ImGui::Text("Name");ImGui::SameLine(150.f);ImGui::InputText("##MaterialName", &_name);

	bool _flag = false;
	ImGui::Text("GraphicShader");ImGui::SameLine(150.f);
	if (ImGui::BeginCombo("##shaderCombo", _shader.c_str()))
	{
		map<wstring, Ptr<Asset>> _shaders = AssetMgr::GetInst()->GetGraphicShader();
		map<wstring, Ptr<Asset>>::iterator iter = _shaders.begin();
		for (;iter != _shaders.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flag))
			{
				_shader = EH::ConvertString(iter->first);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Sprite 0");ImGui::SameLine(150.f);
	if (ImGui::BeginCombo("##Sprite0", _shader.c_str()))
	{
		map<wstring, Ptr<Asset>> _sprites = AssetMgr::GetInst()->GetSprite();
		map<wstring, Ptr<Asset>>::iterator iter = _sprites.begin();
		for (;iter != _sprites.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flag))
			{
				_sprite = EH::ConvertString(iter->first);
			}
		}
		ImGui::EndCombo();
	}
}