#include "pch.h"
#include "EHRendererUI.h"

#include "EHRenderer.h"
#include "EHTileMap.h"
#include "EHGameObject.h"

#include "EHTileMgr.h"

RendererUI::RendererUI()
	:ComponentUI("Renderer", "##Renderer", COMPONENT_TYPE::RENDERER)
	, m_ColFlag(FALSE)
	, m_Collider{}
	, m_CurTile{}
	, m_prevTile{}

{
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
	case RENDERER_TYPE::END:
		break;
	default:
		break;
	}
}

void RendererUI::MeshRenderer_Update()
{

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

}