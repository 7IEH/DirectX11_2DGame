#pragma once
#include "EHUI.h"

class TreeUI;
class TreeNode
{
private:
	TreeUI*				m_Owner;

	string				m_Name;
	string				m_ID;
	vector<TreeNode*>	m_vecChildNode;

	TreeNode*			m_ParentNode;
	DWORD_PTR			m_Data;

	bool				m_bFrame;
	bool				m_bSelected;

public:
	void SetName(string& _name) { m_Name = _name; }
	void SetFrame(bool _frame) { m_bFrame = _frame; }

	const string& GetName() { return m_Name; }
	DWORD_PTR GetData() { return m_Data; }

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
	TreeNode*		m_Selected;

	TreeNode*		m_DragNode;
	TreeNode*		m_DropNode;

	bool            m_bShowRoot;
	bool			m_bDragDrop;

	UI*				m_SelectInst;
	Delegate_1		m_SelectFunc;
	bool			m_bSelectEvent;

	UI*				m_DragDropInst;
	Delegate_2		m_DragDropFunc;
	bool			m_bDragDropEvent;

public:
	virtual void Render_Update() override;

public:
	void AddSelectDelegate(UI* _inst, Delegate_1 _pFunc) { m_SelectInst = _inst; m_SelectFunc = _pFunc; }
	void AddDragDropDelegate(UI* _inst, Delegate_2 _pFunc) { m_DragDropInst = _inst; m_DragDropFunc = _pFunc; }

	void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }
	void UseDragDrop(bool _use) { m_bDragDrop = _use; }

	TreeNode* AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData);
	void ClearNode()
	{
		if (nullptr != m_Root)
		{
			delete m_Root;
			m_Root = nullptr;
		}
	}

private:
	void SetSelectedNode(TreeNode* _selectedNode);
	void SetDragNode(TreeNode* _DragNode);
	void SetDropNode(TreeNode* _DropNode);

public:
	TreeUI(const string& _ID);
	~TreeUI();

	friend class TreeNode;
};

