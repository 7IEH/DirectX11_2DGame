#include "pch.h"
#include "EHGameObject.h"

#include "EHComponent.h"
#include "EHScript.h"

#include "EHGarbageCollector.h"
#include "EHLevelMgr.h"
#include "EHLevel.h"

#include "EHScriptMgr.h"

int GameObject::m_ObjectID = 0;

GameObject::GameObject()
	:m_LayerType(LAYER_TYPE::NON_SELECT)
	, m_Component{}
	, m_vScripts{}
	, m_Renderer(nullptr)
	, m_Dead(false)
	, m_Picking(false)
	, m_Idx(m_ObjectID++)
	, m_bEnabled(TRUE)
{
}

GameObject::GameObject(const GameObject& _origin)
	:Entity(_origin)
	, m_LayerType(_origin.m_LayerType)
	, m_Component{}
	, m_vScripts{}
	, m_Renderer(nullptr)
	, m_Parent(nullptr)
	, m_Dead(false)
	, m_Picking(false)
	, m_Idx(m_ObjectID++)
	, m_bEnabled(TRUE)
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr == _origin.m_Component[i])
			continue;
		AddComponent(_origin.m_Component[i]->Clone());

		if (COMPONENT_TYPE::CAMERA == COMPONENT_TYPE(i))
		{
			Camera* _cam = (Camera*)_origin.m_Component[i];

			if (nullptr != _cam)
			{
				CAMERA_TYPE _type = _cam->GetCameraType();
				this->GetComponent<Camera>(COMPONENT_TYPE::CAMERA)->SetCameraType(_type);
			}
		}
	}

	for (size_t i = 0;i < _origin.m_vScripts.size();i++)
	{
		AddComponent(_origin.m_vScripts[i]->Clone());
	}

	for (size_t i = 0;i < _origin.m_Childs.size();i++)
	{
		if (nullptr != m_Childs[i])
			AddChild(_origin.m_Childs[i]->Clone());
	}
}

GameObject::~GameObject()
{
	delete m_Renderer;
	m_Renderer = nullptr;
	m_Component[(UINT)COMPONENT_TYPE::RENDERER] = nullptr;

	ReleaseArray<Component, (int)COMPONENT_TYPE::END>(m_Component);
	ReleaseVector(m_Childs);
	ReleaseVector(m_vScripts);
}

void GameObject::AddComponent(Component* _comp)
{
	COMPONENT_TYPE _type = _comp->GetType();

	if (_type == COMPONENT_TYPE::SCRIPT)
	{
		// Script Ÿ�� Component �� ������ Script Ŭ������ �ƴ� ���
		assert(dynamic_cast<Script*>(_comp));

		m_vScripts.push_back((Script*)_comp);
		_comp->SetOwner(this);
	}
	else
	{
		// �̹� �ش� Ÿ���� ������Ʈ�� �����ϰ� �ִ� ��� 
		assert(!m_Component[(UINT)_type]);

		m_Component[(UINT)_type] = _comp;
		_comp->SetOwner(this);

		Renderer* pRenderCom = dynamic_cast<Renderer*>(_comp);
		if (nullptr != pRenderCom)
		{
			// �̹� �� ���� �̻��� RenderComponent �� �����ϰ� �ִ� ���
			assert(!m_Renderer);

			m_Renderer = pRenderCom;
		}
	}
}

void GameObject::Awake()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
			m_Component[_comp]->Awake();
	}

	for (size_t _script = 0;_script < m_vScripts.size();_script++)
	{
		if (nullptr != m_vScripts[_script])
			m_vScripts[_script]->Awake();
	}
}

void GameObject::Start()
{
	for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
	{
		if (m_Component[_comp] != nullptr)
			m_Component[_comp]->Start();
	}

	for (size_t _script = 0;_script < m_vScripts.size();_script++)
	{
		if (nullptr != m_vScripts[_script])
			m_vScripts[_script]->Start();
	}
}

void GameObject::Update()
{
	if (m_bEnabled)
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
	}

	for (size_t i = 0; i < m_Childs.size();i++)
	{
		m_Childs[i]->Update();
	}
}

void GameObject::FixedUpdate()
{
	if (m_bEnabled)
	{
		for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
		{
			if (m_Component[_comp] == nullptr)
				m_Component[_comp]->FixedUpdate();
		}
	}
}

void GameObject::LateUpdate()
{
	if (m_bEnabled)
	{
		for (UINT _comp = 0;_comp < (UINT)COMPONENT_TYPE::END;_comp++)
		{
			if (m_Component[_comp] != nullptr)
			{
				m_Component[_comp]->LateUpdate();
			}
		}

		if (!m_vScripts.empty())
		{
			for (Script* _script : m_vScripts)
			{
				_script->LateUpdate();
			}
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
	if (m_bEnabled)
	{
		Renderer* _renderer = dynamic_cast<Renderer*>(m_Renderer);
		if (_renderer != nullptr)
			_renderer->Render();

		for (size_t i = 0; i < m_Childs.size();i++)
		{
			m_Childs[i]->Render();
		}
	}
}

void GameObject::AddChild(GameObject* _child)
{
	if (_child->m_Parent != nullptr)
	{
		_child->DisconnectWithParent();
	}
	else
	{
		LAYER_TYPE _type = _child->m_LayerType;
		_child->DisconnectWithLayer();
		_child->m_LayerType = _type;
	}

	_child->m_Parent = this;
	m_Childs.push_back(_child);
}

int GameObject::DisconnectWithParent()
{
	if (nullptr == m_Parent)
		return -1;

	bool bSuccess = false;

	vector<GameObject*>::iterator iter = m_Parent->m_Childs.begin();

	for (;iter != m_Parent->m_Childs.end();++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_Childs.erase(iter);
			m_Parent = nullptr;
			bSuccess = true;
			break;
		}
	}

	if (!bSuccess)
	{
		assert(nullptr);
	}

	int layerIdx = int(m_LayerType);

	m_LayerType = LAYER_TYPE::NON_SELECT;

	return layerIdx;
}

int GameObject::DisconnectWithLayer()
{
	if (LAYER_TYPE::NON_SELECT == m_LayerType)
		return -1;

	Level* _CurLevel = LevelMgr::GetInst()->GetCurLevel();

	int layerIdx = int(m_LayerType);
	if (nullptr != _CurLevel)
	{
		Layer* _CurLayer = _CurLevel->GetLayer(m_LayerType);
		_CurLayer->DetachGameObject(this);
	}

	return layerIdx;
}

void GameObject::Save(std::wofstream* _file)
{
	if (_file->is_open())
	{
		*_file << L'\n';
		*_file << L"object\n";
		*_file << L"NAME:" + GetName() + L'\n';
		*_file << L"LAYER:" + std::to_wstring(int(GetLayerType())) + L'\n';

		// Component ���� ����
		for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
		{
			if (nullptr != m_Component[i])
				m_Component[i]->Save(_file);
		}

		// Script ���� ����
		*_file << L"Script\n";
		*_file << std::to_wstring(m_vScripts.size()) + L'\n';
		for (size_t i = 0;i < m_vScripts.size();i++)
		{
			*_file << m_vScripts[i]->GetName() + L'\n';
		}

		*_file << L"Childs\n";
		*_file << std::to_wstring(m_Childs.size()) + L'\n';

		for (size_t i = 0;i < m_Childs.size();i++)
		{
			m_Childs[i]->Save(_file);
		}

		*_file << L"endobject\n";
	}
}

void GameObject::Load(std::wifstream* _file, Level* _level, bool _bParent)
{
	wstring _line = L"";

	while (1)
	{
		std::getline(*_file, _line);

		Component* _comp = nullptr;

		if (_line.find(L"NAME") != wstring::npos)
		{
			_line = _line.substr(_line.find(L":") + 1);
			SetName(wstring(_line.begin(), _line.end()));
		}

		if (_line.find(L"LAYER") != wstring::npos)
		{
			_line = _line.substr(_line.find(L":") + 1);
			m_LayerType = LAYER_TYPE(std::stoi(_line));
		}

		if (_line.find(L"TRANSFORM") != string::npos)
		{
			_comp = AddComponent<Transform>();
		}

		if (_line.find(L"LIGHT2D") != string::npos)
		{
			_comp = AddComponent<LIght2D>();
		}

		if (_line.find(L"CAMERA") != string::npos)
		{
			_comp = AddComponent<Camera>();
		}

		if (_line.find(L"MESHRENDERER") != string::npos)
		{
			_comp = AddComponent<MeshRenderer>();
		}

		if (_line.find(L"CANVASRENDERER") != string::npos)
		{
			_comp = AddComponent<CanvasRenderer>();
		}

		if (_line.find(L"RIGIDBODY") != string::npos)
		{
			_comp = AddComponent<RigidBody>();
		}

		if (_line.find(L"PARTICLESYSTEM") != string::npos)
		{
			_comp = AddComponent<ParticleSystem>();
		}

		if (_line.find(L"ANIMATOR2D") != string::npos)
		{
			_comp = AddComponent<Animator2D>();
		}

		if (_line.find(L"BOXCOLLIDER2D") != string::npos)
		{
			_comp = AddComponent<Collider2D>();
		}

		if (_line.find(L"CIRCLECOLLIDER2D") != string::npos)
		{
			_comp = AddComponent<CircleCollider2D>();
		}

		if (_line.find(L"Script") != string::npos)
		{
			std::getline(*_file, _line);
			size_t _size = std::stoi(_line);

			for (size_t i = 0;i < _size;i++)
			{
				Script* _script = nullptr;
				std::getline(*_file, _line);
				if (L"Light2DScript" == _line)
					continue;

				_script = ScriptMgr::GetInst()->FindScript(_line);
				assert(_script);
				_script = _script->Clone();
				AddComponent(_script);
			}
		}

		if (_line.find(L"TEXT") != string::npos)
		{
			_comp = AddComponent<Text>();
		}

		if (_line.find(L"BUTTON") != string::npos)
		{
			_comp = AddComponent<Button>();
		}

		if (_line.find(L"Childs") != wstring::npos)
		{
			std::getline(*_file, _line);
			int _size = std::stoi(_line);

			for (int i = 0;i < _size;i++)
			{
				GameObject* _child = new GameObject;
				_child->Load(_file, _level, TRUE);
				AddChild(_child);
			}
		}

		if (_line.find(L"endobject") != wstring::npos)
		{
			if (!_bParent)
			{
				_level->AddObject(this, this->GetLayerType(), FALSE);
			}

			break;
		}

		if (nullptr != _comp)
			_comp->Load(_file);
	}
}