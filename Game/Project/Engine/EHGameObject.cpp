#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"
#include "EHScript.h"

#include "EHGarbageCollector.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

GameObject::GameObject()
	:m_LayerType(LAYER_TYPE::NON_SELECT)
	, m_Component{}
	, m_Renderer(nullptr)
	, m_vScripts{}
	, m_Parent(nullptr)
	, m_Dead(true)
{
}

GameObject::~GameObject()
{
	ReleaseArray<Component, (int)COMPONENT_TYPE::END>(m_Component);
	ReleaseVector(m_Childs);
	ReleaseVector(m_vScripts);
}

void GameObject::Awake()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
			m_Component[_comp]->Awake();
	}
}

void GameObject::Start()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] == nullptr)
			m_Component[_comp]->Start();
	}
}

void GameObject::Update()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
		{
			m_Component[_comp]->Update();
		}
	}

	if (!m_vScripts.empty())
	{
		for (Script* _script : m_vScripts)
		{
			_script->Update();
		}
	}

	for (size_t i = 0; i < m_Childs.size();i++)
	{
		m_Childs[i]->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] == nullptr)
			m_Component[_comp]->FixedUpdate();
	}
}

void GameObject::LateUpdate()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
		{
			m_Component[_comp]->LateUpdate();
		}
	}

	Layer* pCurLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(m_LayerType);
	pCurLayer->RegisterGameObject(this);

	vector<GameObject*>::iterator iter = m_Childs.begin();

	for (;iter != m_Childs.end();)
	{
		(*iter)->LateUpdate();

		if ((*iter)->m_Dead)
		{
			GarbageCollector::GetInst()->AddExitObject((*iter));
			iter = m_Childs.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GameObject::Render()
{
	Renderer* _renderer = dynamic_cast<Renderer*>(m_Renderer);
	if (_renderer != nullptr)
		_renderer->Render();

	for (size_t i = 0; i < m_Childs.size();i++)
	{
		m_Childs[i]->Render();
	}
}

void GameObject::AddChild(GameObject* _child)
{
	if (_child->m_Parent != nullptr)
	{
		_child->DisconnectWithParent();
	}

	_child->m_Parent = this;
	m_Childs.push_back(_child);
}

void GameObject::DisconnectWithParent()
{
	vector<GameObject*>::iterator iter = m_Parent->m_Childs.begin();

	for (;iter != m_Childs.end();++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_Childs.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}
	
	assert(nullptr);
}

void GameObject::DisconnectWithLayer()
{
	if (LAYER_TYPE::NON_SELECT == m_LayerType)
		return;

	Level* _CurLevel = LevelMgr::GetInst()->GetCurLevel();
	Layer* _CurLayer = _CurLevel->GetLayer(m_LayerType);
	_CurLayer->DetachGameObject(this);
}