#include "pch.h"
#include "EHAddComponent.h"

#include "EHGameObject.h"
#include "EHDevice.h"

#include "EHBehaviour.h"
#include "EHInspector.h"

AddComponent::AddComponent()
	:UI("AddCompnentUI", "##AddCompnentUI")
{
}

AddComponent::~AddComponent()
{
}

void AddComponent::Render_Update()
{
	float _width = Device::GetInst()->GetResolution().x * 0.15625f;
	float _height = Device::GetInst()->GetResolution().y * 0.027f;
	ImGui::SameLine(90.f);
	if (ImGui::Button("AddCompnent", ImVec2(_width,_height)) || m_Open)
	{
		Inspector* _inspc = (Inspector*)ImGUIMgr::GetInst()->FindUI("Inspector");
		m_Open = TRUE;
		if (ImGui::BeginMenu)
		{
			if (ImGui::MenuItem("Transform"))
			{
				m_TargetObject->AddComponent<Transform>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("Camera"))
			{
				m_TargetObject->AddComponent<Camera>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("Light2D"))
			{
				m_TargetObject->AddComponent<LIght2D>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("BoxCollider2D"))
			{
				m_TargetObject->AddComponent<Collider2D>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("CircleCollider2D"))
			{
				m_TargetObject->AddComponent<CircleCollider2D>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("MeshRenderer"))
			{
				m_TargetObject->AddComponent<MeshRenderer>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("TileMap"))
			{
				m_TargetObject->AddComponent<TileMap>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("ParticleSystem"))
			{
				m_TargetObject->AddComponent<ParticleSystem>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("Animator2D"))
			{
				m_TargetObject->AddComponent<Animator2D>();
				m_Open = FALSE;
				_inspc->SetTargetObject(m_TargetObject);
			}

			if (ImGui::MenuItem("Script"))
			{
				/*m_TargetObject->AddComponent<Transform>();
				m_Open = FALSE;*/
			}

			if (ImGui::MenuItem("EXIT"))
			{
				m_Open = FALSE;
			}
		}
	}
}