#include "pch.h"
#include "EHHierarchy.h"

#include "EHLevelMgr.h"
#include "EHLevel.h"
#include "EHLayer.h"
#include "EHGameObject.h"

#include "EHInspector.h"
#include "EHTreeUI.h"

Hierarchy::Hierarchy()
	:UI("Hierarch", "##Hierarchy")
{
	m_Tree = new TreeUI("HierarchyTree");
	m_Tree->ShowRootNode(false);
	m_Tree->UseDragDrop(true);
	AddChildUI(m_Tree);

	m_Tree->AddSelectDelegate(this, (Delegate_1)&Hierarchy::SelectObject);
	m_Tree->AddDragDropDelegate(this, (Delegate_2)&Hierarchy::DragDropObject);

	ResetCurrentLevel();
}

Hierarchy::~Hierarchy()
{
}

/********************
| Object Picking
********************/

void Hierarchy::ResetCurrentLevel()
{
	m_Tree->ClearNode();

	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

	Level* _curScene = LevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		Layer* _layer = _curScene->GetLayer(LAYER_TYPE(i));
		const vector<GameObject*>& _vecParent = _layer->GetLayerParent();

		for (size_t j = 0;j < _vecParent.size();j++)
		{
			AddObjectToTree(pRootNode, _vecParent[j]);
		}
	}
}

void Hierarchy::AddObjectToTree(TreeNode* _node, GameObject* _object)
{
	TreeNode* pNode = m_Tree->AddTreeNode(_node, EH::ConvertString(_object->GetName()), (DWORD_PTR)_object);

	const vector<GameObject*>& _vecChild = _object->GetChild();

	for (size_t i = 0;i < _vecChild.size();i++)
	{
		AddObjectToTree(pNode, _vecChild[i]);
	}
}

void Hierarchy::SelectObject(DWORD_PTR _node)
{
	TreeNode* _pNode = (TreeNode*)_node;
	GameObject* _object = (GameObject*)_pNode->GetData();

	if (nullptr == _object)
		return;

	Inspector* _inspec = (Inspector*)ImGUIMgr::GetInst()->FindUI("Inspector");
	_inspec->SetTargetObject(_object);
}

void Hierarchy::DragDropObject(DWORD_PTR _dest, DWORD_PTR _source)
{
	TreeNode* _destNode = (TreeNode*)_dest;
	TreeNode* _sourceNode = (TreeNode*)_source;

	if (nullptr == _destNode)
	{
		GameObject* _sourceObj = (GameObject*)_sourceNode->GetData();

		int LayerIdx = _sourceObj->DisconnectWithParent();

		if (LayerIdx == -1)
			return;

		Level* _curScene = LevelMgr::GetInst()->GetCurLevel();
		_curScene->AddObject(_sourceObj, LAYER_TYPE(LayerIdx), FALSE);
	}
	else
	{
		GameObject* _destObj = (GameObject*)_destNode->GetData();
		GameObject* _sourceObj = (GameObject*)_sourceNode->GetData();
		_destObj->AddChild(_sourceObj);
	}

	ResetCurrentLevel();
}