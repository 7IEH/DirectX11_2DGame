#include "pch.h"
#include "EHUI.h"

UI::UI(const string& _strName, const string& _strID)
	: m_label(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
{
}

UI::~UI()
{
}

void UI::Update()
{

}

void UI::Render()
{
	if (!m_bActive)
		return;

	if (nullptr == GetParentUI())
	{
		ImGui::Begin(string(m_label + m_strID).c_str(), &m_bActive);

		Render_Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Render();
		}

		ImGui::End();
	}

	else
	{
		if (m_Collapsing)
		{
			if (ImGui::CollapsingHeader(string(m_label + m_strID).c_str()))
			{
				Render_Update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Render();
				}
			}
		}
		else
		{
			ImGui::BeginChild(string(m_label + m_strID).c_str());

			Render_Update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Render();
			}

			ImGui::EndChild();
		}
	}
}