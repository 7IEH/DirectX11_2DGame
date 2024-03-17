#include "pch.h"
#include "EHLevel.h"

#include "EHCollisionMgr.h"
#include "EHAssetMgr.h"

#include "EHGameObject.h"
#include "EHLayer.h"

#include "EHLevelMgr.h"

#include "EHImGUIMgr.h"
#include "EHHierarchy.h"

Level::Level()
	:m_Layers{}
	, m_CollistionMatrix{}
	, m_CopyCount(0)
	, m_bEditor(TRUE)
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type] = new Layer;
		m_Layers[_type]->m_Type = LAYER_TYPE(_type);
		m_CollistionMatrix[_type] = 0;
	}
}

Level::Level(const Level& _origin)
	: m_Layers{}
	, m_CollistionMatrix{}
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_Layers[i] = _origin.m_Layers[i]->Clone();
	}

	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_CollistionMatrix[i] = _origin.m_CollistionMatrix[i];
	}
}

Level::~Level()
{
	ReleaseArray(m_Layers);
}

GameObject* Level::FindObjectByName(const wstring& _strName)
{
	GameObject* _obj = nullptr;
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		_obj = m_Layers[i]->FindObject(_strName);

		if (_obj != nullptr)
			break;
	}

	return _obj;
}

Level* Level::CopyLevel()
{
	Level* _copyLevel = this->Clone();
	wstring _copyName = this->GetName() + std::to_wstring(m_CopyCount++);
	LevelMgr::GetInst()->AddLevel(_copyName, _copyLevel);

	return _copyLevel;
}

void Level::FindObjectsByName(const wstring& _strName, vector<GameObject*>& _vecObj)
{

}

void Level::Load(string _path)
{
	// UTF-8 ÇÑ±Û ±úÁü ÇØ°á
	std::locale::global(std::locale(".UTF-8"));

	std::wifstream _file(string(_path.begin(), _path.end()).data());

	if (_file.is_open())
	{
		bool _temp = false;
		wstring _line = L"";

		std::getline(_file, _line);

		if (_line == L"CollisionMatrix")
		{
			for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
			{
				std::getline(_file, _line);
				m_CollistionMatrix[i] = std::stoi(_line);
			}
		}

		while (std::getline(_file, _line))
		{
			if (_line == L"object")
			{
				GameObject* _obj = new GameObject;
				_obj->Load(&_file, this, FALSE);
			}
		}
		_file.close();
	}
}

void Level::SetCamera()
{
	Layer* _camLayer = m_Layers[(UINT)LAYER_TYPE::CAMERA];
	vector<GameObject*> _objs = _camLayer->GetLayerParent();

	for (size_t i = 0;i < _objs.size();i++)
	{
		Camera* _cam = _objs[i]->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);
		if (nullptr == _cam)
			continue;

		_cam->SetCameraType(_cam->GetCameraType());
		_cam->AllVisibleSet(TRUE);
	}
}

void Level::Awake()
{
#ifdef _DEBUG
	if (m_bEditor)
	{
		if (nullptr == GetLayer(LAYER_TYPE::CAMERA)->FindObject(L"EditorCamera"))
		{
			GameObject* _editorCam = new GameObject;
			_editorCam->SetName(L"EditorCamera");
			_editorCam->AddComponent<Transform>();
			Camera* _cam = _editorCam->AddComponent<Camera>();
			_cam->SetCameraType(CAMERA_TYPE::WORLD_CAMERA);
			_cam->AllVisibleSet(TRUE);
			_cam->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);

			_editorCam->AddComponent<CameraScript>();

			AddObject(_editorCam, LAYER_TYPE::CAMERA, FALSE);
		}
	}
#endif
	SetCamera();

	CollisionMgr::GetInst()->SetCollisionMatrix(m_CollistionMatrix);

	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Awake();
	}
	Start();
}

void Level::Start()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Start();
	}
}

void Level::Update()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Update();
	}
}

void Level::FixedUpdate()
{

}

void Level::LateUpdate()
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->LateUpdate();
	}
}

void Level::Clear()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_Layers[i]->m_GameObjects.clear();
	}
}
