#include "pch.h"
#include "EHMaterialCreator.h"
#include "EHAssetMgr.h"
#include "EHPathMgr.h"

MaterialCreator::MaterialCreator()
	:UI("MaterialCreator", "##MaterialCreator")
	, m_sName{}
	, m_sShader{}
	, m_sSprite{}
	, m_wName{}
	, m_wShader{}
	, m_wSprite{}
{
}

MaterialCreator::~MaterialCreator()
{
}

void MaterialCreator::Render_Update()
{
	if (ImGui::Button("Create"))
	{
		wstring _wrealtivePath = PATH;
		wstring _wPath = _wrealtivePath + L"\\resource\\materialdata\\Material.txt";
		std::wofstream _pFile(_wPath.c_str(), std::ios::out | std::ios::app);

		m_wName = EH::ConvertWstring(m_sName);
		m_wShader = EH::ConvertWstring(m_sShader);
		m_wSprite = EH::ConvertWstring(m_sSprite);

		_pFile << m_wName + L'\n';
		_pFile << m_wShader + L"\n";
		_pFile << m_wSprite + L"\n";

		// assetmgr 실행 시 material 만들기도 바꿈
		Material* _mat = new Material;
		_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(m_wShader));
		_mat->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Sprite>(m_wSprite));
		AssetMgr::GetInst()->AddAsset<Material>(_mat, m_wName);
	}

	if (ImGui::Button("Exit"))
	{
		// 나가기
	}

	ImGui::Text("Name");ImGui::SameLine(150.f);ImGui::InputText("##MaterialName", &m_sName);

	bool _flag = false;
	ImGui::Text("GraphicShader");ImGui::SameLine(150.f);
	if (ImGui::BeginCombo("##shaderCombo", m_sShader.c_str()))
	{
		map<wstring, Ptr<Asset>> _shaders = AssetMgr::GetInst()->GetGraphicShader();
		map<wstring, Ptr<Asset>>::iterator iter = _shaders.begin();
		for (;iter != _shaders.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flag))
			{
				m_sShader = EH::ConvertString(iter->first);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Sprite 0");ImGui::SameLine(150.f);
	if (ImGui::BeginCombo("##Sprite0", m_sSprite.c_str()))
	{
		map<wstring, Ptr<Asset>> _sprites = AssetMgr::GetInst()->GetSprite();
		map<wstring, Ptr<Asset>>::iterator iter = _sprites.begin();
		for (;iter != _sprites.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flag))
			{
				m_sSprite = EH::ConvertString(iter->first);
			}
		}
		ImGui::EndCombo();
	}
}