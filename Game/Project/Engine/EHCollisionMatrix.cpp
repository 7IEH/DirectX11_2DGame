#include "pch.h"
#include "EHCollisionMatrix.h"

#include "EHCollisionMgr.h"
#include "EHImGUIMgr.h"

CollisionMatrix::CollisionMatrix()
	:UI("CollisionMatrix", "##CollisionMatrix")
	,m_CollisionMatrix{}
	,m_MaxLength(0)
{
	m_LayerName.push_back("Camera");
	m_LayerName.push_back("Light2D");
	m_LayerName.push_back("BackGround");
	m_LayerName.push_back("Player");
	m_LayerName.push_back("Monster");
	m_LayerName.push_back("Trigger");
	m_LayerName.push_back("Tile");
	m_LayerName.push_back("Particle");
}

CollisionMatrix::~CollisionMatrix()
{
}


void CollisionMatrix::Render_Update()
{
	UINT* _matrix = CollisionMgr::GetInst()->GetCollisionMatrix();

	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_CollisionMatrix[i] = _matrix[i];

		if (m_MaxLength < m_LayerName[i].size())
		{
			m_MaxLength = (UINT)m_LayerName[i].size();
		}
	}

	string s = "";
	for (UINT i = 0;i < m_MaxLength;i++)
	{
		for (int j = (int)LAYER_TYPE::END - 1;j >= 0;j--)
		{
			if (j == (int)LAYER_TYPE::END-1)
			{
				ImGui::Text(" ");
				ImGui::SameLine(135.f);
			}

			if (i < m_LayerName[j].size())
			{
				s = m_LayerName[j][i];
				ImGui::Text(s.c_str());

				if (j != 0)
					ImGui::SameLine(135.f + 32.f * (6 - j));
			}
			else
			{
				s = " ";
				ImGui::Text(s.c_str());
				if (j != 0)
					ImGui::SameLine(135.f + 32.f * (6 - j));
			}
		}
	}


	for (UINT _x = 0;_x < (UINT)LAYER_TYPE::END;_x++)
	{
		bool _flag = false;
		ImGui::Text(m_LayerName[_x].c_str());

		ImGui::SameLine(130.f);

		char _label[30] = {};

		for (UINT _y = (UINT)LAYER_TYPE::END - 1;_y >= _x;_y--)
		{
			if (m_CollisionMatrix[_x] & (1 << _y))
			{
				_flag = true;

				sprintf_s(_label, "##Label%d", 10 * _x + _y);

				ImGui::Checkbox(_label, &_flag);
				ImGui::SameLine();

				if (_flag)
				{
					CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE(_x), LAYER_TYPE(_y));
				}
				else
				{
					CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE(_x), LAYER_TYPE(_y), false);
				}
			}
			else
			{
				_flag = false;

				sprintf_s(_label, "##Label%d", 10 * _x + _y);

				ImGui::Checkbox(_label, &_flag);
				ImGui::SameLine();

				if (_flag)
				{
					CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE(_x), LAYER_TYPE(_y));
				}
				else
				{
					CollisionMgr::GetInst()->LayerCheck(LAYER_TYPE(_x), LAYER_TYPE(_y), false);
				}
			}

			if (_y == 0)
				break;
		}
		ImGui::Text("");
	}
}