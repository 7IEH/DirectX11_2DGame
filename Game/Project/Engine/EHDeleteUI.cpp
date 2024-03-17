#include "pch.h"
#include "EHDeleteUI.h"

#include "EHPathMgr.h"
#include "EHLevelMgr.h"
#include "EHImGUIMgr.h"

DeleteUI::DeleteUI()
	:UI("DeleteUI", "##DeleteUI")
	, m_GameOjbect{}
{
}

DeleteUI::~DeleteUI()
{
}

void DeleteUI::Render_Update()
{
	ImGui::InputText("##SceneCreateUI", &m_GameOjbect);
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		wstring _name = EH::ConvertWstring(m_GameOjbect);
		GameObject* _obj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(_name);

		if(nullptr != _obj)
			Object::Destroy(_obj);
	}
}
