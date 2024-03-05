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
		// Script 타입 Component 가 실제로 Script 클래스가 아닌 경우
		assert(dynamic_cast<Script*>(_comp));

		m_vScripts.push_back((Script*)_comp);
		_comp->SetOwner(this);
	}
	else
	{
		// 이미 해당 타입의 컴포넌트를 보유하고 있는 경우 
		assert(!m_Component[(UINT)_type]);

		m_Component[(UINT)_type] = _comp;
		_comp->SetOwner(this);

		Renderer* pRenderCom = dynamic_cast<Renderer*>(_comp);
		if (nullptr != pRenderCom)
		{
			// 이미 한 종류 이상의 RenderComponent 를 보유하고 있는 경우
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
	Layer* _CurLayer = _CurLevel->GetLayer(m_LayerType);

	int layerIdx = int(m_LayerType);
	_CurLayer->DetachGameObject(this);

	return layerIdx;
}

void GameObject::Save(string _path)
{
	std::ofstream _file;
	_file.open(_path.data(), std::fstream::out | std::fstream::app);

	std::ofstream _filename;

	if (_file.is_open())
	{
		string _name = "NAME:" + EH::ConvertString(GetName()) + '\n';
		string _layer = "LAYER:" + std::to_string(int(GetLayerType())) + '\n';
		string _temp = "";
		_temp = "object\n";
		_file.write("\n", 1);
		_file.write(_temp.c_str(), _temp.size());
		_file.write(_name.c_str(), _name.size());
		_file.write(_layer.c_str(), _layer.size());
		_file.close();

		// Component 정보 저장
		for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
		{
			if (nullptr != m_Component[i])
				m_Component[i]->Save(_path);
		}

		_file.open(_path.data(), std::fstream::out | std::fstream::app);

		// Script 정보 저장
		_file << "Script\n";
		_file << std::to_string(m_vScripts.size()) + '\n';
		for (size_t i = 0;i < m_vScripts.size();i++)
		{
			_file << EH::ConvertString(m_vScripts[i]->GetName()) + '\n';
		}
		_temp = "endobject\n";
		_file.write(_temp.c_str(), _temp.size());
	}

	_file << "Childs\n";
	_file << std::to_string(m_Childs.size()) + '\n';

	_file.close();

	for (size_t i = 0;i < m_Childs.size();i++)
	{
		m_Childs[i]->Save(_path);
	}

}

void GameObject::Load(std::wifstream* _file, Level* _level)
{
	wstring _line = L"";

	while (1)
	{
		std::getline(*_file, _line);

		if (_line.find(L"NAME") != wstring::npos)
		{
			_line = _line.substr(_line.find(L":") + 1);
			SetName(wstring(_line.begin(), _line.end()));
		}

		if (_line.find(L"LAYER") != wstring::npos)
		{
			_line = _line.substr(_line.find(L":") + 1);
			LAYER_TYPE _type = LAYER_TYPE(std::stoi(_line));
			_level->AddObject(this, _type, FALSE);
		}

		if (_line.find(L"TRANSFORM") != string::npos)
		{
			Transform* _tr = AddComponent<Transform>();
			_tr->Load(_file);
		}

		if (_line.find(L"LIGHT2D") != string::npos)
		{
			LIght2D* _light = AddComponent<LIght2D>();
			_light->Load(_file);
		}

		if (_line.find(L"CAMERA") != string::npos)
		{
			Camera* _cam = AddComponent<Camera>();
			_cam->Load(_file);
		}

		if (_line.find(L"MESHRENDERER") != string::npos)
		{
			MeshRenderer* _render = AddComponent<MeshRenderer>();
			_render->Load(_file);
		}

		if (_line.find(L"CANVASRENDERER") != string::npos)
		{
			CanvasRenderer* _render = AddComponent<CanvasRenderer>();
			_render->Load(_file);
		}

		if (_line.find(L"PARTICLESYSTEM") != string::npos)
		{
			ParticleSystem* _render = AddComponent<ParticleSystem>();
			_render->Load(_file);
		}

		if (_line.find(L"ANIMATOR2D") != string::npos)
		{
			Animator2D* _animator = AddComponent<Animator2D>();
			_animator->Load(_file);
		}

		if (_line.find(L"BOXCOLLIDER2D") != string::npos)
		{
			Collider2D* _col = AddComponent<Collider2D>();
			_col->Load(_file);
		}

		if (_line.find(L"CIRCLECOLLIDER2D") != string::npos)
		{
			CircleCollider2D* _col = AddComponent<CircleCollider2D>();
			_col->Load(_file);
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
			Text* _txt = AddComponent<Text>();
			_txt->Load(_file);
		}

		if (_line.find(L"BUTTON") != string::npos)
		{
			Button* _btn = AddComponent<Button>();
			_btn->Load(_file);
		}

		if (_line.find(L"endobject") != wstring::npos)
		{
			break;
		}
	}
}