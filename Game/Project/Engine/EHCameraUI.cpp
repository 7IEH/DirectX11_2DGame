#include "pch.h"
#include "EHCameraUI.h"

#include "EHGameObject.h"
#include "EHCamera.h"

CameraUI::CameraUI()
	:ComponentUI("Camera","##Camera",COMPONENT_TYPE::CAMERA)
{
	m_NameByLayer.insert(make_pair("Perspective", PROJECTION_TYPE::PERSPECTIVE));
	m_NameByLayer.insert(make_pair("Orthographic", PROJECTION_TYPE::ORTHOGRAPHIC));

	m_LayerByName.insert(make_pair(PROJECTION_TYPE::PERSPECTIVE,"Perspective"));
	m_LayerByName.insert(make_pair(PROJECTION_TYPE::ORTHOGRAPHIC,"Orthographic"));

	m_LayerName.push_back("Perspective");
	m_LayerName.push_back("Orthographic");

	m_LayerSelect.insert(make_pair("Perspective", FALSE));
	m_LayerSelect.insert(make_pair("Orthographic", FALSE));
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Update()
{
	if (nullptr == GetTargetObject())
		return;

	Camera* _cam = GetTargetObject()->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);

	PROJECTION_TYPE _type = _cam->GetProjectionType();
	
	map<PROJECTION_TYPE, string>::iterator iter = m_LayerByName.find(_type);

	string _curProejction = iter->second;

	map<string, bool>::iterator iter2;

	ImGui::Text(_curProejction.c_str());
	ImGui::SameLine();
	if (ImGui::BeginCombo("##ProjectionType", _curProejction.c_str()))
	{
		for (UINT _idx = 0; _idx < (UINT)PROJECTION_TYPE::END;_idx++)
		{
			bool is_selected = (_curProejction == m_LayerName[_idx]);
			iter2 = m_LayerSelect.find(m_LayerName[_idx]);
			if (ImGui::Selectable(m_LayerName[_idx].c_str(), iter2->second))
				_curProejction = m_LayerName[_idx].c_str();
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	map<string, PROJECTION_TYPE>::iterator iter3 = m_NameByLayer.find(_curProejction);

	_cam->SetPorjectionType(iter3->second);
	
	float _FOV = _cam->GetFOV();
	float _Far = _cam->GetFar();

	float _scale = _cam->GetScale();

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
	{
		ImGui::Text("Field of View");
		ImGui::SameLine();
		ImGui::DragFloat("##FOV", &_FOV, 0.3f);

		ImGui::Text("Clipping Planes");
		ImGui::SameLine();
		ImGui::Text("Far");
		ImGui::SameLine();
		ImGui::DragFloat("##Far", &_Far, 0.3f);
	}
	else
	{
		ImGui::Text("Size");
		ImGui::SameLine();
		ImGui::DragFloat("##size", &_scale, 0.3f);

		ImGui::Text("Clipping Planes");
		ImGui::SameLine();
		ImGui::Text("Far");
		ImGui::SameLine();
		ImGui::DragFloat("##Far", &_Far, 0.3f);
	}

	_cam->SetFOV(_FOV);
	_cam->SetFar(_Far);
	_cam->SetScale(_scale);
}