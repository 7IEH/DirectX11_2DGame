#pragma once

#include "EHUI.h"

class TreeNode
{
private:
	string				m_Name;
	string				m_ID;
	vector<TreeNode*>	m_vecChildNode;

	TreeNode*			m_ParentNode;
	DWORD_PTR			m_Data;

public:
	void SetName(string& _name) { m_Name = _name; }

private:
	void SetID(const string& _id) { m_ID = _id; }
	void AddChildNode(TreeNode* _childNode)
	{
		m_vecChildNode.push_back(_childNode);
		_childNode->m_ParentNode = this;
	}

public:
	void Render_Update();

public:
	TreeNode();
	~TreeNode();


	friend class TreeUI;
};

class TreeUI
	:public UI
{
private:
	static  UINT    NodeID;

private:
	TreeNode*		m_Root;
	bool            m_bShowRoot;

public:
	virtual void Render_Update() override;

public:
	void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }
	TreeNode* AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData);
	void ClearNode()
	{
		if (nullptr != m_Root)
		{
			delete m_Root;
			m_Root = nullptr;
		}
	}

public:
	TreeUI(const string& _ID);
	~TreeUI();

};

