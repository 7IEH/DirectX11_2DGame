#include "pch.h"
#include "EHCameraUI.h"

#include "EHGameObject.h"
#include "EHCamera.h"

CameraUI::CameraUI()
	:ComponentUI("Camera", "##Camera", COMPONENT_TYPE::CAMERA)
{
	m_NameByLayer.insert(make_pair("Perspective", PROJECTION_TYPE::PERSPECTIVE));
	m_NameByLayer.insert(make_pair("Orthographic", PROJECTION_TYPE::ORTHOGRAPHIC));

	m_LayerByName.insert(make_pair(PROJECTION_TYPE::PERSPECTIVE, "Perspective"));
	m_LayerByName.insert(make_pair(PROJECTION_TYPE::ORTHOGRAPHIC, "Orthographic"));

	m_LayerName.push_back("Perspective");
	m_LayerName.push_back("Orthographic");

	m_LayerSelect.insert(make_pair("Perspective", FALSE));
	m_LayerSelect.insert(make_pair("Orthographic", FALSE));

	m_VisibleLayer.insert(make_pair((UINT)LAYER_TYPE::BACKGROUND, "BackGround"));
	m_VisibleLayer.insert(make_pair((UINT)LAYER_TYPE::PLAYER, "Player"));
	m_VisibleLayer.insert(make_pair((UINT)LAYER_TYPE::MONSTER, "Monster"));
	m_VisibleLayer.insert(make_pair((UINT)LAYER_TYPE::TRIGGER, "Trigger"));
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

	ImGui::Text("Projection");
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

	// Both
	float _Far = _cam->GetFar();

	// 원근
	float _FOV = _cam->GetFOV();

	// 직교
	float _scale = _cam->GetScale();
	float _width = _cam->GetWidth();

	// Visible Layer
	UINT _layerVisible = _cam->GetLayerVisible();

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
	{
		ImGui::Text("Field of View");
		ImGui::SameLine(170.f);
		ImGui::DragFloat("##FOV", &_FOV, 0.3f);

		ImGui::Text("Clipping Planes");
		ImGui::SameLine();
		ImGui::Text("Far");
		ImGui::SameLine(170.f);
		ImGui::DragFloat("##Far", &_Far, 0.3f);

		if (ImGui::CollapsingHeader("Visible Layer"))
		{
			map<UINT, string>::iterator iter;
			bool _flag = false;

			for (UINT _layer = 2;_layer < (UINT)LAYER_TYPE::END;_layer++)
			{
				iter = m_VisibleLayer.find(_layer);
				if (_layerVisible & (1 << _layer))
				{
					_flag = true;
					ImGui::Checkbox(iter->second.c_str(), &_flag);
					_cam->LayerVisibleSet(LAYER_TYPE(_layer), _flag);
				}
				else
				{
					_flag = false;
					ImGui::Checkbox(iter->second.c_str(), &_flag);
					_cam->LayerVisibleSet(LAYER_TYPE(_layer), _flag);
				}
			}
		}
	}
	else
	{
		ImGui::Text("Size");
		ImGui::SameLine(170.f);
		ImGui::DragFloat("##size", &_scale, 0.3f);

		ImGui::Text("Width");
		ImGui::SameLine(170.f);
		ImGui::DragFloat("##width", &_width);

		ImGui::Text("Clipping Planes");
		ImGui::SameLine();
		ImGui::Text("Far");
		ImGui::SameLine();
		ImGui::DragFloat("##Far", &_Far, 0.3f);

		if (ImGui::CollapsingHeader("Visible Layer"))
		{
			map<UINT, string>::iterator iter;
			bool _flag = false;

			for (UINT _layer = 2;_layer < (UINT)LAYER_TYPE::END;_layer++)
			{
				iter = m_VisibleLayer.find(_layer);
				if (_layerVisible & (1 << _layer))
				{
					_flag = true;
					ImGui::Checkbox(iter->second.c_str(), &_flag);
					_cam->LayerVisibleSet(LAYER_TYPE(_layer), _flag);
				}
				else
				{
					_flag = false;
					ImGui::Checkbox(iter->second.c_str(), &_flag);
					_cam->LayerVisibleSet(LAYER_TYPE(_layer), _flag);
				}
			}
		}
	}

	// Both
	_cam->SetFar(_Far);

	// 원근
	_cam->SetFOV(_FOV);

	// 직교
	_cam->SetScale(_scale);
	_cam->SetWidth(_width);
}