#include "pch.h"
#include "EHLevel.h"

#include "EHCollisionMgr.h"
#include "EHAssetMgr.h"

#include "EHGameObject.h"
#include "EHLayer.h"

#include "EHScriptMgr.h"
#include "Scripts.h"
#include "EHLevelMgr.h"

Level::Level()
	:m_Layers{}
	, m_CollistionMatrix{}
	, m_CopyCount(0)
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
	std::ifstream _file(string(_path.begin(), _path.end()).data());

	if (_file.is_open())
	{
		bool _temp = false;
		string _line = "";

		std::getline(_file, _line);

		if (_line == "CollisionMatrix")
		{
			for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
			{
				std::getline(_file, _line);
				m_CollistionMatrix[i] = std::stoi(_line);
			}
		}

		while (std::getline(_file, _line))
		{
			if (_line == "object")
			{
				GameObject* _obj = new GameObject;
				while (1)
				{
					std::getline(_file, _line);

					if (_line.find("NAME") != string::npos)
					{
						_line = _line.substr(_line.find(":") + 1);
						_obj->SetName(wstring(_line.begin(), _line.end()));
					}

					if (_line.find("LAYER") != string::npos)
					{
						_line = _line.substr(_line.find(":") + 1);
						LAYER_TYPE _type = LAYER_TYPE(std::stoi(_line));
						AddObject(_obj, _type);
					}

					if (_line.find("TRANSFORM") != string::npos)
					{
						Vec4 _postion = (Vec4)0.f;
						Vec4 _scale = (Vec4)1.f;
						Vec3 _rotation = (Vec3)0.f;

						for (int i = 0;i < 3;i++)
						{
							std::getline(_file, _line);
							int _prev = 0;
							int _next = 0;
							string _temp = "";

							if (i == 0)
							{
								EH::InputVector3(_line, _postion);
							}
							else if (i == 1)
							{
								EH::InputVector3(_line, _scale);
							}
							else
							{
								EH::InputVector3(_line, _rotation);
							}
						}

						AddTrasnform(_obj, _postion, _scale, _rotation);
					}

					if (_line.find("LIGHT2D") != string::npos)
					{
						LIGHT_TYPE _type = LIGHT_TYPE::DIRECTIONAL;
						Vec4 _color = Vec4(0.f);
						Vec4 _ambient = Vec4(0.f);
						float _angle = 0.f;
						float _radius = 0.f;

						for (int i = 0;i < 5;i++)
						{
							std::getline(_file, _line);
							if (i == 0)
							{
								_type = LIGHT_TYPE(stoi(_line));
							}
							else if (i == 1)
							{
								EH::InputVector4(_line, _color);
							}
							else if (i == 2)
							{
								EH::InputVector4(_line, _ambient);
							}
							else if (i == 3)
							{
								_angle = stof(_line);
							}
							else
							{
								_radius = stof(_line);
							}
						}

						AddLight2D(_obj, _type, _color, _ambient, _angle, _radius);
					}

					if (_line.find("CAMERA") != string::npos)
					{
						PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;
						CAMERA_TYPE _camType = CAMERA_TYPE::MAIN_CAMERA;
						UINT _visibleLayer = 0;

						for (int i = 0;i < 3;i++)
						{
							std::getline(_file, _line);
							if (i == 0)
							{
								if ("perspective" != _line)
								{
									_type = PROJECTION_TYPE::ORTHOGRAPHIC;
								}
							}
							else if (i == 1)
							{
								_camType = CAMERA_TYPE(stoi(_line));
							}
							else
							{
								_visibleLayer = (UINT)stoi(_line);
							}
						}

						AddCamera(_obj, _type, _camType, _visibleLayer);
					}

					if (_line.find("MESHRENDERER") != string::npos)
					{
						wstring _mesh = L"";;
						wstring _material = L"";

						for (int i = 0;i < 2;i++)
						{
							std::getline(_file, _line);
							if (i == 0)
							{
								_mesh = EH::ConvertWstring(_line);
							}
							else
							{
								_material = EH::ConvertWstring(_line);
							}
						}

						AddMeshRenderer(_obj, _mesh, _material);
					}

					if (_line.find("ANIMATOR2D") != string::npos)
					{
						size_t _size = 0;
						vector<wstring>_aniName; _aniName.clear();

						//size name
						std::getline(_file, _line);

						_size = std::stoi(_line);

						for (size_t i = 0;i < _size;i++)
						{
							std::getline(_file, _line);
							_aniName.push_back(EH::ConvertWstring(_line));
						}

						AddAnimator2D(_obj, _aniName);
					}

					if (_line.find("BOXCOLLIDER2D") != string::npos)
					{
						Vec3 _offsetPos = Vec3(0.f);
						Vec3 _offsetSize = Vec3(0.f);

						for (int i = 0;i < 2;i++)
						{
							std::getline(_file, _line);
							if (i == 0)
							{
								EH::InputVector3(_line, _offsetSize);
							}
							else
							{
								EH::InputVector3(_line, _offsetPos);
							}
						}

						AddCollider2D(_obj, _offsetPos, _offsetSize);
					}

					if (_line.find("CIRCLECOLLIDER2D") != string::npos)
					{
						Vec3 _offsetPos = Vec3(0.f);
						float _radius = 0.f;

						for (int i = 0;i < 2;i++)
						{
							std::getline(_file, _line);
							if (i == 0)
							{
								_radius = std::stof(_line);
							}
							else
							{
								EH::InputVector3(_line, _offsetPos);
							}
						}

						AddCollider2D(_obj, _offsetPos, _radius);
					}

					if (_line.find("Script") != string::npos)
					{
						vector<wstring>_script;_script.clear();
						int _size = 0;
						std::getline(_file, _line);
						_size = std::stoi(_line);

						for (int i = 0;i < _size;i++)
						{
							std::getline(_file, _line);
							_script.push_back(EH::ConvertWstring(_line));
						}

						AddScript(_obj, _script);
					}

					if (_line.find("endobject") != string::npos)
					{
						break;
					}
				}
			}
		}
		_file.close();
	}
}

void Level::Awake()
{
	CollisionMgr::GetInst()->SetCollisionMatrix(m_CollistionMatrix);

	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_Layers[_type]->Awake();
	}
}

void Level::Start()
{

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

void Level::AddTrasnform(GameObject* _obj, Vec4 _position, Vec4 _scale, Vec3 _rotation)
{
	Transform* _tr = _obj->AddComponent<Transform>();
	_tr->SetRelativePosition(_position);
	_tr->SetRelativeScale(_scale);
	_tr->SetRelativeRotation(_rotation);
}

void Level::AddCamera(GameObject* _obj, PROJECTION_TYPE _proj, CAMERA_TYPE _camtype, UINT _visibleLayer)
{
	Camera* _cam = _obj->AddComponent<Camera>();
	_cam->SetPorjectionType(_proj);
	_cam->SetCameraType(_camtype);

	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		if (_visibleLayer & (1 << i))
		{
			_cam->LayerVisibleSet(LAYER_TYPE(i), TRUE);
		}
		else
		{
			_cam->LayerVisibleSet(LAYER_TYPE(i), FALSE);
		}
	}
}

void Level::AddLight2D(GameObject* _obj, LIGHT_TYPE _lighttype, Vec4 _color, Vec4 _ambient, float _angle, float _radius)
{
	LIght2D* _light = _obj->AddComponent<LIght2D>();
	_obj->AddComponent<Light2DScript>();
	_light->SetLightType(_lighttype);
	_light->SetColor(_color);
	_light->SetAmbient(_ambient);
	_light->SetAngle(_angle);
	_light->SetRadius(_radius);
}

void Level::AddMeshRenderer(GameObject* _obj, wstring _mesh, wstring _material)
{
	MeshRenderer* _mr = _obj->AddComponent<MeshRenderer>();
	_mr->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(_mesh));
	_mr->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(_material));
}

void Level::AddCollider2D(GameObject* _obj, Vec3 _offsetPostion, Vec3 _offsetScale)
{
	Collider2D* _col = _obj->AddComponent<Collider2D>();
	_col->SetOffsetPos(_offsetPostion);
	_col->SetoffSetScale(_offsetScale);
}

void Level::AddCollider2D(GameObject* _obj, Vec3 _offsetPostion, float _radius)
{
	CircleCollider2D* _col = _obj->AddComponent<CircleCollider2D>();
	_col->SetOffsetPos(_offsetPostion);
	_col->SetRadius(_radius);
}

void Level::AddAnimator2D(GameObject* _obj, vector<wstring> _aniName)
{
	Animator2D* _anim = _obj->AddComponent<Animator2D>();

	for (size_t i = 0;i < _aniName.size();i++)
	{
		if (FAILED(_anim->AddAnimation2D(_aniName[i])))
		{
			return;
		}
	}
}

void Level::AddScript(GameObject* _obj, vector<wstring>_scripts)
{
	Script* _script = nullptr;
	for (size_t i = 0;i < _scripts.size();i++)
	{
		_script = ScriptMgr::GetInst()->FindScript(_scripts[i]);
		assert(_script);
		_script = _script->Clone();
		_obj->AddComponent(_script);
	}
}