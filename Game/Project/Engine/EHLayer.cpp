#include "pch.h"
#include "EHLayer.h"

#include "EHGameObject.h"
#include "EHGarbageCollector.h"

Layer::Layer()
	:m_Type(LAYER_TYPE::NON_SELECT)
{
}

Layer::Layer(const Layer& _origin)
	:m_Parent{}
	, m_GameObjects{}
	, m_Type(_origin.m_Type)
{
	for (size_t i = 0;i < _origin.m_Parent.size();i++)
	{
		m_Parent.push_back(_origin.m_Parent[i]->Clone());
	}

	// GameObject �ذ�
}

Layer::~Layer()
{
	ReleaseVector(m_Parent);
}

void Layer::Awake()
{
	for (size_t i = 0;i < m_Parent.size();i++)
	{
		m_Parent[i]->Awake();
	}
}

void Layer::Start()
{
	for (size_t i = 0;i < m_Parent.size();i++)
	{
		m_Parent[i]->Start();
	}
}

void Layer::Update()
{
	for (size_t i = 0;i < m_Parent.size();i++)
	{
		m_Parent[i]->Update();
	}
}

void Layer::FixedUpdate()
{
	for (size_t i = 0;i < m_Parent.size();i++)
	{
		m_Parent[i]->FixedUpdate();
	}
}

void Layer::LateUpdate()
{
	vector<GameObject*>::iterator iter = m_Parent.begin();
	for (;iter != m_Parent.end();)
	{
		if (nullptr == (*iter))
			continue;

		(*iter)->LateUpdate();

		if ((*iter)->m_Dead)
		{
			GarbageCollector::GetInst()->AddExitObject((*iter));
			iter = m_Parent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Layer::AddObject(GameObject* _obj, bool _bMove)
{
	// _bMove : ture - ���̾ �ԷµǴ� Object�� �ڽ��� �ִ� ���, �ڽ� ���� ��� �ش� ���̾�� �Ѿ�´�.
	// _bMove : false - ���̾ �ԷµǴ� Object�� �ڽ��� �ش� ���̾�� ���� �Ѿ���� �ʴ´�. �� �ڽĿ�����Ʈ�� ������ �Ҽ��� ���� ��� (-1) ���� ���� ����ȴ�.

	// �ֻ��� �θ� ������Ʈ����.
	if (_obj->GetParent() == nullptr)
	{
		if (_obj->GetLayerType() != LAYER_TYPE::NON_SELECT)
		{
			_obj->DisconnectWithLayer();
		}

		m_Parent.push_back(_obj);
	}

	// ������Ʈ�� ���̾� �Ҽ��� ���� ���̾�� ����
	// �ڽ� ������Ʈ���� ���̾� �Ҽ��� ����
	std::queue<GameObject*> _q;
	_q.push(_obj);

	while (!_q.empty())
	{
		GameObject* _temp = _q.front(); _q.pop();

		for (size_t i = 0;i < _temp->m_Childs.size();++i)
		{
			_q.push(_temp->m_Childs[i]);
		}

		if (_temp == _obj)
			_temp->m_LayerType = m_Type;
		else
		{
			if (_bMove)
			{
				_temp->m_LayerType = m_Type;
			}
			else if (_temp->m_LayerType == LAYER_TYPE::NON_SELECT)
			{
				_temp->m_LayerType = m_Type;
			}
		}
	}
}

void Layer::DetachGameObject(GameObject* _obj)
{
	assert(!(LAYER_TYPE::NON_SELECT == _obj->m_LayerType || m_Type != _obj->m_LayerType));

	if (_obj->GetParent() != nullptr)
	{
		_obj->m_LayerType = LAYER_TYPE::NON_SELECT;
	}
	else
	{
		vector<GameObject*>::iterator iter = m_Parent.begin();
		for (;iter != m_Parent.end();++iter)
		{
			if (*iter == _obj)
			{
				m_Parent.erase(iter);
				_obj->m_LayerType = LAYER_TYPE::NON_SELECT;
				return;
			}
		}
	}

	assert(nullptr);
}

void Layer::Save(string _path)
{
	for (size_t i = 0;i < m_Parent.size();i++)
	{
		if (m_Parent[i] != nullptr)
			m_Parent[i]->Save(_path);
	}
}