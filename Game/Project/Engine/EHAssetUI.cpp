#include "pch.h"
#include "EHAssetUI.h"


AssetUI::AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _type)
	:UI(_strName,_ID)
	,m_Type(_type)
{
	Enabled(FALSE);
}

AssetUI::~AssetUI()
{
}


void AssetUI::SetAsset(Ptr<Asset> _asset)
{
	m_Asset = _asset;

	if (nullptr == m_Asset)
	{
		Enabled(FALSE);
	}

	else
	{
		if (_asset->GetType() == m_Type)
		{
			Enabled(TRUE);
		}
		else
		{
			Enabled(FALSE);
		}
	}
}

void AssetUI::Render_Update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));

	ImGui::Button(ASSET_TYPE_STRING[(UINT)m_Type]);

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}