#pragma once

#include "EHUI.h"

class TreeUI;
class TreeNode;
class Inspector;
class Hierarchy
	:public UI
{
private:
	TreeUI* m_Tree;
	Inspector* m_Inspector;
	
public:
	void ResetCurrentLevel();

public:
	void SetInspector(Inspector* _inspector) { m_Inspector = _inspector; }
	Inspector* GetInsepctor() { return m_Inspector; }

private:
	void AddObjectToTree(TreeNode* _node, GameObject* _object);
	void SelectObject(DWORD_PTR _node);
	void DragDropObject(DWORD_PTR _dest, DWORD_PTR _source);

public:
	virtual void Render_Update() {};

public:
	Hierarchy();
	virtual ~Hierarchy();
};

