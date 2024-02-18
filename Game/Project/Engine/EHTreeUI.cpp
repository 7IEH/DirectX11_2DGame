#include "pch.h"
#include "EHTreeUI.h"

#include "EHKeyMgr.h"

TreeNode::TreeNode()
	:m_bFrame(false)
{
}

TreeNode::~TreeNode()
{
	ReleaseVector(m_vecChildNode);
}

void TreeNode::Render_Update()
{
	string _strID = m_Name + m_ID;

	UINT FLAG = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (m_bFrame)
	{
		FLAG |= ImGuiTreeNodeFlags_Framed;
	}
	if (m_vecChildNode.empty())
	{
		FLAG |= ImGuiTreeNodeFlags_Leaf;
	}
	if (m_bSelected)
	{
		FLAG |= ImGuiTreeNodeFlags_Selected;
	}

	if (m_bFrame && m_vecChildNode.empty())
		_strID = _strID;

	if (ImGui::TreeNodeEx(_strID.c_str(),FLAG))
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			m_Owner->SetDragNode(this);
		}

		else if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());
			if (payload)
			{
				m_Owner->SetDropNode(this);
			}
			ImGui::EndDragDropTarget();
		}

		else
		{
			if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				m_Owner->SetSelectedNode(this);
			}
		}

		if (ImGui::IsItemClicked())
		{
			m_Owner->SetSelectedNode(this);
		}

		for (size_t i = 0;i < m_vecChildNode.size();i++)
		{
			m_vecChildNode[i]->Render_Update();
		}

		ImGui::TreePop();
	}
}


UINT TreeUI::NodeID = 0;

TreeUI::TreeUI(const string& _ID)
	:UI("", _ID)
	, m_bShowRoot(true)
	, m_bDragDrop(false)
{
}

TreeUI::~TreeUI()
{
	delete m_Root;
	m_Root = nullptr;
}


void TreeUI::Render_Update()
{
	if (nullptr == m_Root)
		return;


	if (m_bShowRoot)
	{
		m_Root->Render_Update();
	}

	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
		{
			m_Root->m_vecChildNode[i]->Render_Update();
		}
	}

	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}

	// 드래그 대상을 특정 노드가 아닌 공중드랍 시킨 경우
	if (KEY_RELEASED(KEY::LBTN) && m_DragNode && !m_DropNode)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}
		m_DragNode = nullptr;
	}
	else if (m_bDragDropEvent)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}

		m_DropNode = nullptr;
		m_DragNode = nullptr;
	}

	m_bSelectEvent = false;
	m_bDragDropEvent = false;
}

TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode;
	pNewNode->m_Data = _dwData;
	pNewNode->SetName(_strName);

	// 노드마다 겹치지않는 숫자를 ## 뒤에 ID 로 붙인다.
	UINT id = NodeID++;

	char buff[50] = {};
	sprintf_s(buff, "##%d", id);
	pNewNode->SetID(buff);

	if (nullptr == _Parent)
	{
		assert(!m_Root);

		m_Root = pNewNode;
	}
	else
	{
		_Parent->AddChildNode(pNewNode);
	}

	pNewNode->m_Owner = this;

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _selectedNode)
{
	// 전에 눌린 Selected UI FALSE 해주고
	if (m_Selected)
	{
		m_Selected->m_bSelected = FALSE;
	}

	// 현재 선택된 노드
	m_Selected = _selectedNode;

	// m_Selected 업어지지 않았다면
	if (nullptr != m_Selected)
	{
		m_Selected->m_bSelected = TRUE;
	}

	m_bSelectEvent = TRUE;
}

void TreeUI::SetDragNode(TreeNode* _DragNode)
{
	m_DragNode = _DragNode;
}

void TreeUI::SetDropNode(TreeNode* _DropNode)
{
	m_DropNode = _DropNode;
	m_bDragDropEvent = true;
}