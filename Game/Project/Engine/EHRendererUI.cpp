#include "pch.h"
#include "EHRendererUI.h"

#include "EHRenderer.h"
#include "EHTileMap.h"
#include "EHGameObject.h"

#include "EHAssetMgr.h"
#include "EHTileMgr.h"
#include "EHParticleSystem.h"

RendererUI::RendererUI()
	:ComponentUI("Renderer", "##Renderer", COMPONENT_TYPE::RENDERER)
	, m_ColFlag(FALSE)
	, m_Collider{}
	, m_CurTile{}
	, m_prevTile{}
{
	m_Noise = FALSE;
}

RendererUI::~RendererUI()
{
}

void RendererUI::Render_Update()
{
	GameObject* _object = GetTargetObject();

	Renderer* _renderer = GetTargetObject()->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER);

	switch (_renderer->GetRenderType())
	{
	case RENDERER_TYPE::MESHRENDERER:
	{
		MeshRenderer_Update();
	}
	break;
	case RENDERER_TYPE::TILEMAP:
	{
		TileMap_Update();
	}
	break;
	case RENDERER_TYPE::PARTICLESYSTEM:
	{
		Particle_Update();
	}
	break;
	case RENDERER_TYPE::CANVAS:
	{
		Canvas_Update();
	}
	break;
	case RENDERER_TYPE::END:
		break;
	default:
		break;
	}
}

void RendererUI::MeshRenderer_Update()
{
	ImGui::Text("Mesh");ImGui::SameLine(100.f);

	string _mesh = " ";
	if (nullptr == GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMesh())
	{
		_mesh = " ";
	}
	else
	{
		_mesh = EH::ConvertString(GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMesh()->GetName());
	}

	if (ImGui::BeginCombo("##Mesh", _mesh.c_str()))
	{
		map<wstring, Ptr<Asset>> _meshs = AssetMgr::GetInst()->GetMesh();
		map<wstring, Ptr<Asset>>::iterator iter = _meshs.begin();

		bool _flags = false;
		for (;iter != _meshs.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flags))
			{
				GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(iter->first));
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Text("Material");ImGui::SameLine(100.f);

	string _material = " ";
	if (nullptr == GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial())
	{
		_material = " ";
	}
	else
	{
		_material = EH::ConvertString(GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->GetMaterial()->GetName());
	}

	if (ImGui::BeginCombo("##Material", _material.c_str()))
	{
		map<wstring, Ptr<Asset>> _materials = AssetMgr::GetInst()->GetMaterial();
		map<wstring, Ptr<Asset>>::iterator iter = _materials.begin();

		bool _flags = false;
		for (;iter != _materials.end();iter++)
		{
			if (ImGui::Selectable(EH::ConvertString(iter->first).c_str(), _flags))
			{
				GetTargetObject()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER)->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(iter->first));
			}
		}

		ImGui::EndCombo();
	}
}

void RendererUI::TileMap_Update()
{
	map<wstring, vector<tTilePaletteInfo>>& _tileInfo = TileMgr::GetInst()->GetTileAllInfo();

	map<wstring, vector<tTilePaletteInfo>>::iterator iter = _tileInfo.begin();

	if (_tileInfo.size() == 0)
		return;
	/******************
	| Select Tile
	******************/
	bool _flag = false;

	if (m_CurTile == "")
	{
		m_CurTile = "Empty";
		m_prevTile = "Empty";
	}

	if (ImGui::BeginCombo("##TileMapList", m_CurTile.c_str()))
	{
		for (;iter != _tileInfo.end();iter++)
		{
			string _label = string(iter->first.begin(), iter->first.end());

			if (ImGui::Selectable(_label.c_str(), &_flag))
			{
				m_CurTile = _label.c_str();
			}
		}
		ImGui::EndCombo();
	}

	if (m_prevTile != m_CurTile && m_CurTile != "")
	{
		m_prevTile = m_CurTile;
		iter = _tileInfo.find(wstring(m_CurTile.begin(), m_CurTile.end()));
		TileMap* _tm = GetTargetObject()->GetComponent<TileMap>(COMPONENT_TYPE::RENDERER);
		vector<tTilePaletteInfo> _tile = iter->second;

		if (iter == _tileInfo.end())
			return;

		if (_tile.size() == 0)
			return;

		_tm->SetFace((UINT)_tile[0]._Face.x, (UINT)_tile[0]._Face.y);
		_tm->SetRenderTileSize(_tile[0]._RenderSize);
		_tm->SetTileAtlas(_tile[0]._atlas, _tile[0]._PixelSize);

		for (UINT _col = 0;_col < (UINT)_tile[0]._Face.y;_col++)
		{
			for (UINT _row = 0; _row < (UINT)_tile[0]._Face.x;_row++)
			{
				int _idx = _row * (int)_tile[0]._Face.x + _col;
				_tm->SetTileIndex(_row, _col, _tile[_idx]._TileIdx, _tile[_idx]._Render);
			}
		}
	}

	/******************
	| Collider Apply
	******************/
	ImGui::Separator();
	ImGui::Text("Apply Collider");ImGui::SameLine();ImGui::Checkbox("##ApplyCollider", &m_ColFlag);

	if (m_ColFlag && m_Collider.size() == 0)
	{
		// Collider »ý¼º
		iter = _tileInfo.find(wstring(m_CurTile.begin(), m_CurTile.end()));
		TileMap* _tm = GetTargetObject()->GetComponent<TileMap>(COMPONENT_TYPE::RENDERER);
		vector<tTilePaletteInfo> _tile = iter->second;

		GameObject* _owenr = GetTargetObject();
		Transform* _ownerTr = _owenr->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

		Vec4 _Scale = _ownerTr->GetRelativeScale();

		Vec2 _st = Vec2(-_Scale.x / 2.f, -_Scale.y / 2.f);

		for (UINT _col = 0;_col < (UINT)_tile[0]._Face.y;_col++)
		{
			for (UINT _row = 0; _row < (UINT)_tile[0]._Face.x;_row++)
			{
				int _idx = _row * (int)_tile[0]._Face.x + _col;

				if (_tile[_idx]._ColliderEnabled == 1)
				{
					GameObject* _obj = new GameObject();
					Transform* _tr = _obj->AddComponent<Transform>();

					Vec2 _CalPos = {};
					_CalPos.x = _st.x + _tile[0]._RenderSize.x * _row + _tile[0]._RenderSize.x / 2.f;
					_CalPos.y = _st.y + _tile[0]._RenderSize.y * _col + _tile[0]._RenderSize.y / 2.f;

					_tr->SetRelativePosition(Vec4(_CalPos.x, _CalPos.y, 0.f, 1.f));
					_tr->SetRelativeScale(Vec4(_tile[0]._RenderSize.x, _tile[0]._RenderSize.y, 1.f, 1.f));
					_obj->AddComponent<Collider2D>();
					_obj->SetLayerType(LAYER_TYPE::TILE);
					wstring _strName = L"TileCollider" + std::to_wstring(_obj->GetID());
					_obj->SetName(_strName);
					_owenr->AddChild(_obj);
					m_Collider.push_back(_obj);
				}
			}
		}

	}
	else if (!m_ColFlag)
	{
		GameObject* DisconnectWithParent = GetTargetObject();
		// Collider ÆÄ±«
		for (size_t i = 0;i < m_Collider.size();i++)
		{
			m_Collider[i]->DisconnectWithParent();
			Object::Destroy(m_Collider[i]);
		}

		m_Collider.clear();
	}
}

void RendererUI::Particle_Update()
{
	ParticleSystem* _particle = GetTargetObject()->GetComponent<ParticleSystem>(COMPONENT_TYPE::RENDERER);
	tParticleModule _module = _particle->GetModule();

	// Basic Module
	if (ImGui::TreeNode("Basic Module"))
	{
		float	_duration = 0.f;
		bool	_looping = false;
		float	_startDelay = 0.f;
		float	_startSpeed = 0.f;
		float	_startSize = 0.f;
		float	_startRotation = 0.f;
		float	_StartColor[3] = { _module._SpawnColor.x,_module._SpawnColor.y,_module._SpawnColor.z };
		int		_maxParticles = 0;

		ImGui::Text("Duration");		 ImGui::SameLine(150.f); ImGui::DragFloat("##Duration", &_duration);
		ImGui::Text("Looping");			 ImGui::SameLine(150.f); ImGui::Checkbox("##Looping", &_looping);
		ImGui::Text("Start Delay");		 ImGui::SameLine(150.f); ImGui::DragFloat("##Start Delay", &_startDelay);
		ImGui::Text("Min LifeTime");	 ImGui::SameLine(150.f); ImGui::DragFloat("##Min LifeTime", &_module._MinLife);
		ImGui::Text("Max LifeTime");	 ImGui::SameLine(150.f); ImGui::DragFloat("##Max LifeTime", &_module._MaxLife);
		ImGui::Text("Start Size");		 ImGui::SameLine(150.f); ImGui::DragFloat("##Start Size", &_startSize);
		ImGui::Text("Start Rotation");	 ImGui::SameLine(150.f); ImGui::DragFloat("##Start Rotation", &_startRotation);
		ImGui::Text("Start Color");		 ImGui::SameLine(150.f); ImGui::ColorEdit3("##StartColor", _StartColor);
		ImGui::Text("Max Particles");	 ImGui::SameLine(150.f); ImGui::DragInt("##Max Particles", &_maxParticles);
		ImGui::Text("Simulation Space"); ImGui::SameLine(180.f);

		bool _flag = false;
		vector<string>_space;
		_space.push_back("Local");
		_space.push_back("World");

		for (int i = 0;i < _space.size();i++)
		{
			if (i == _module._SpaceType)
			{
				m_CurSpace = _space[i];
			}
		}

		if (ImGui::BeginCombo("##SimulationSpace", m_CurSpace.c_str()))
		{
			for (int i = 0;i < _space.size();i++)
			{
				if (ImGui::Selectable(_space[i].c_str(), &_flag))
				{
					_module._SpaceType = 0;
				}
			}
			ImGui::EndCombo();
		}

		vector<string>_shape;
		_shape.push_back("Circle");
		_shape.push_back("Box");

		for (int i = 0;i < _shape.size();i++)
		{
			if (i == _module._SpawnShape)
			{
				m_CurShape = _shape[i];
			}
		}

		ImGui::Text("SpawnShape"); ImGui::SameLine(180.f);
		if (ImGui::BeginCombo("##SpawnShape", m_CurShape.c_str()))
		{
			for (int i = 0;i < _shape.size();i++)
			{
				if (ImGui::Selectable(_shape[i].c_str(), &_flag))
				{
					_module._SpawnShape = i;
				}
			}
			ImGui::EndCombo();
		}

		if (_module._SpawnShape == 0)
		{
			ImGui::Text("Radius"); ImGui::SameLine(150.f); ImGui::DragFloat("##Radius", &_module._Radius);
		}
		else if (_module._SpawnShape == 1)
		{
			float _boxScale[4] = { _module._SpawnBoxScale.x,_module._SpawnBoxScale.y ,_module._SpawnBoxScale.z ,_module._SpawnBoxScale.w };
			ImGui::Text("BoxScale"); ImGui::SameLine(150.f);ImGui::DragFloat4("##BosScale", _boxScale);
			_module._SpawnBoxScale = Vec4{ _boxScale[0],_boxScale[1],_boxScale[2],_boxScale[3] };
		}

		map<wstring, Ptr<Asset>> _sprites = AssetMgr::GetInst()->GetSprite();
		map<wstring, Ptr<Asset>>::iterator iter = _sprites.begin();

		_flag = false;
		ImGui::Text("Random Seed(Noise)"); ImGui::SameLine(180.f);
		if (ImGui::BeginCombo("##Random Seed(Noise)", m_CurNoise.c_str()))
		{
			for (;iter != _sprites.end();iter++)
			{
				string _label = string(iter->first.begin(), iter->first.end());
				if (ImGui::Selectable(_label.c_str(), _flag))
				{
					Ptr<Sprite> _sprite = dynamic_cast<Sprite*>(iter->second.Get());
					_particle->SetNoiseSprite(_sprite);
					m_CurNoise = _label;
				}
			}
			ImGui::EndCombo();
		}

		_module._SpawnColor = Vec4(_StartColor[0], _StartColor[1], _StartColor[2], _module._SpawnColor.w);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Renderer"))
	{
		float _minParticleSize[4] = { _module._SpawnMinScale.x,_module._SpawnMinScale.y ,_module._SpawnMinScale.z ,_module._SpawnMinScale.w };
		float _maxParticleSize[4] = { _module._SpawnMaxScale.x,_module._SpawnMaxScale.y ,_module._SpawnMaxScale.z ,_module._SpawnMaxScale.w };

		ImGui::Text("Min Particle Size");ImGui::SameLine(170.f);ImGui::DragFloat4("##Min Particle Size", _minParticleSize);
		ImGui::Text("Max Particle Size");ImGui::SameLine(170.f);ImGui::DragFloat4("##Max Particle Size", _maxParticleSize);

		_module._SpawnMinScale = Vec4(_minParticleSize[0], _minParticleSize[1], _minParticleSize[2], _minParticleSize[3]);
		_module._SpawnMaxScale = Vec4(_maxParticleSize[0], _maxParticleSize[1], _maxParticleSize[2], _maxParticleSize[3]);

		map<wstring, Ptr<Asset>> _sprites = AssetMgr::GetInst()->GetSprite();
		map<wstring, Ptr<Asset>>::iterator iter = _sprites.begin();

		bool _flag = false;
		ImGui::Text("Sprite"); ImGui::SameLine(170.f);
		if (ImGui::BeginCombo("##Sprite", m_CurSprite.c_str()))
		{
			for (;iter != _sprites.end();iter++)
			{
				string _label = string(iter->first.begin(), iter->first.end());
				if (ImGui::Selectable(_label.c_str(), _flag))
				{
					Ptr<Sprite> _sprite = dynamic_cast<Sprite*>(iter->second.Get());
					_particle->SetParticleSprite(_sprite);
					m_CurSprite = _label;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}

	// Custom Module
	ImGui::Checkbox("##ScaleCheck", &m_Scale);ImGui::SameLine();
	if (ImGui::TreeNode("Scale Over LifeTime"))
	{
		float _ratio[4] = { _module._ScaleRatio.x,_module._ScaleRatio.y ,_module._ScaleRatio.z ,_module._ScaleRatio.w };
		ImGui::Text("ScaleRatio"); ImGui::SameLine(150.f); ImGui::DragFloat4("##ScaleRatio", _ratio, 0.3f, 0.f, 1.f);
		_module._ScaleRatio = { _ratio[0],_ratio[1],_ratio[2],_ratio[3] };
		ImGui::TreePop();
	}

	ImGui::Checkbox("##VelocityCheck", &m_Velocity);ImGui::SameLine();
	if (ImGui::TreeNode("Velocity"))
	{
		ImGui::Text("Min Speed"); ImGui::SameLine(150.f); ImGui::DragFloat("##Min Speed", &_module._MinSpeed);
		ImGui::Text("Max Speed"); ImGui::SameLine(150.f); ImGui::DragFloat("##Max Speed", &_module._MaxSpeed);
		ImGui::Text("Velocity Type"); ImGui::SameLine(150.f); ImGui::InputInt("##VelocityType", &_module._AddVelocityType);

		ImGui::TreePop();
	}

	ImGui::Checkbox("##NoiseCheck", &m_Noise);ImGui::SameLine();
	if (ImGui::TreeNode("Noise Velocity"))
	{
		ImGui::Text("Noise Scale");ImGui::SameLine(170.f);ImGui::DragFloat("##NoiseScale", &_module.NoiseForceScale);
		ImGui::Text("Noise Term");ImGui::SameLine(170.f);ImGui::DragFloat("##NoiseTerm", &_module.NoiseForceTerm);
		ImGui::TreePop();
	}

	ImGui::Checkbox("##ColorCheck", &m_Color);ImGui::SameLine();
	if (ImGui::TreeNode("Color Over Lifetime"))
	{
		ImGui::Text("Color Type");ImGui::SameLine();ImGui::InputInt("##ColorType", &_module._ColorType);
		ImGui::TreePop();
	}

	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = m_Velocity;
	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = m_Scale;
	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = m_Noise;
	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = m_Noise;
	_module._arrModuleCheck[(UINT)PARTICLE_MODULE::COLOR_OVER_LIFETIME] = m_Color;

	_particle->SetModule(_module);
}

void RendererUI::Canvas_Update()
{

}