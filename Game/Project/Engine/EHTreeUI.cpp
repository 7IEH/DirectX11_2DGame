#include "pch.h"
#include "EHTreeUI.h"

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
    ReleaseVector(m_vecChildNode);
}

void TreeNode::Render_Update()
{
    string _strID = m_Name + m_ID;

    if (ImGui::TreeNode(_strID.c_str()))
    {
        for (size_t i = 0;i < m_vecChildNode.size();i++)
        {
            m_vecChildNode[i]->Render_Update();
        }

        ImGui::TreePop();
    }
}


UINT TreeUI::NodeID = 0;

TreeUI::TreeUI(const string& _ID)
    :UI("",_ID)
    ,m_bShowRoot(true)
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

    return pNewNode;
}