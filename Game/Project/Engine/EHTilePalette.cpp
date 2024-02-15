#include "pch.h"
#include "EHTilePalette.h"

#include "EHAssetMgr.h"
#include "EHImGUIMgr.h"
#include "EHTileMgr.h"

TilePalette::TilePalette()
	:UI("TileMapPalette", "##TileMapPalette")
	, m_buttonWidth(40.f)
	, m_CurrentLabel{}
	, m_TileInfo{}
	, m_Face{}
	, m_RenderSize{}
	, m_PixelSize{}
	, m_ClickedTile{}
	, m_prevFace{}
{
	m_ClickedTile._tileIdx = -1;
}

TilePalette::~TilePalette()
{
	m_TileInfo.clear();
}

/*********************************************
||---------------------------------------|
||										 |
||				Tile Map				 |
||										 |
||_______________________________________|
|
||--------------|		|----------------|
||				|		|				 |
||	 Setting	|		|  Image		 |
||				|		|				 |
||______________|		|________________|
*********************************************/
void TilePalette::Render_Update()
{
	if (ImGui::Button("Create##TilePalette"))
	{
		wstring _label = wstring(m_TileName.begin(), m_TileName.end());
		TileMgr::GetInst()->AddTileInfo(_label, m_TileInfo);
	}

	ImGui::SameLine();

	if (ImGui::Button("Exit##TilePalette"))
	{
		ImGUIMgr::GetInst()->SetTilePalette(FALSE);
	}

	map<wstring, Ptr<Asset>> _spriteInfo = AssetMgr::GetInst()->GetSprite();
	map<wstring, Ptr<Asset>>::iterator iter = _spriteInfo.begin();

	Ptr<Sprite> _atlasImage = nullptr;

	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(1200.f, 600.f));
	if (ImGui::BeginChild("AtlasImageChild", ImVec2(-FLT_MIN, 600.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		ImGui::Text("Tile Atlas Image"); ImGui::SameLine();
		bool _flag = false;
		if (ImGui::BeginCombo("##TilePalette", m_CurrentLabel.c_str()))
		{
			for (;iter != _spriteInfo.end();iter++)
			{
				bool is_selected = (m_CurrentLabel == string(iter->first.begin(), iter->first.end()));
				string _comboChild = string(iter->first.begin(), iter->first.end());
				if (ImGui::Selectable(_comboChild.c_str(), _flag))
				{
					m_CurrentLabel = _comboChild;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		iter = _spriteInfo.find(wstring(m_CurrentLabel.begin(), m_CurrentLabel.end()));

		if (iter == _spriteInfo.end())
		{
			ImGui::EndChild();
			return;
		}

		_atlasImage = dynamic_cast<Sprite*>(iter->second.Get());

		/********************
		|	Canvas
		********************/
		Canvas(_atlasImage);
	}
	ImGui::EndChild();

	/********************
	|	Setting Palette
	********************/
	Setting(_atlasImage); ImGui::SameLine(); Palette(_atlasImage);
}

void TilePalette::Canvas(Ptr<Sprite> _atlasImage)
{
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;
	static bool adding_line = false;

	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);

	if (m_Face[0] == 0 || m_Face[1] == 0 || m_PixelSize[0] == 0.f || m_PixelSize[1] == 0.f)
	{
		return;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && is_hovered)
	{
		if (m_ClickedTile._Collider == 1)
		{
			int maxRow = int((float)_atlasImage->GetSpriteWidth() / m_PixelSize[0]);
			int maxCol = int((float)_atlasImage->GetSpriteHeight() / m_PixelSize[1]);

			int _row = int(mouse_pos_in_canvas.x / m_PixelSize[0]);
			int _col = int(mouse_pos_in_canvas.y / m_PixelSize[1]);

			int _idx = _col * m_Face[0] + _row;

			if (_idx >= m_TileInfo.size())
			{
				return;
			}

			m_TileInfo[_idx]._ColliderEnabled = 1;
		}
		else
		{
			if (m_ClickedTile._tileIdx == -1)
				return;

			int maxRow = int((float)_atlasImage->GetSpriteWidth() / m_PixelSize[0]);
			int maxCol = int((float)_atlasImage->GetSpriteHeight() / m_PixelSize[1]);

			int _row = int(mouse_pos_in_canvas.x / m_PixelSize[0]);
			int _col = int(mouse_pos_in_canvas.y / m_PixelSize[1]);

			int _idx = _col * m_Face[0] + _row;

			if (_idx >= m_TileInfo.size())
			{
				return;
			}

			m_TileInfo[_idx]._TileIdx = m_ClickedTile._tileIdx;
			m_TileInfo[_idx]._atlas = _atlasImage;
			m_TileInfo[_idx]._Face = Vec2((float)m_Face[0], (float)m_Face[1]);
			m_TileInfo[_idx]._Render = 1;
			m_TileInfo[_idx]._RenderSize = Vec2(m_RenderSize[0], m_RenderSize[1]);
			m_TileInfo[_idx]._PixelSize = Vec2(m_PixelSize[0], m_PixelSize[1]);
		}
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && is_hovered)
	{
		if (m_ClickedTile._tileIdx == -1)
			return;

		int maxRow = int((float)_atlasImage->GetSpriteWidth() / m_PixelSize[0]);
		int maxCol = int((float)_atlasImage->GetSpriteHeight() / m_PixelSize[1]);

		int _row = int(mouse_pos_in_canvas.x / m_PixelSize[0]);
		int _col = int(mouse_pos_in_canvas.y / m_PixelSize[1]);

		int _idx = _col * m_Face[0] + _row;

		if (_idx >= m_TileInfo.size())
		{
			return;
		}

		m_TileInfo[_idx]._TileIdx = 0;
		m_TileInfo[_idx]._atlas = nullptr;
		m_TileInfo[_idx]._Face = Vec2(0.f, 0.f);
		m_TileInfo[_idx]._Render = 0;
		m_TileInfo[_idx]._RenderSize = Vec2(0.f, 0.f);
		m_TileInfo[_idx]._PixelSize = Vec2(0.f, 0.f);
		m_TileInfo[_idx]._ColliderEnabled = 0;
	}

	// 타일 그리기
	for (int i = 0;i < m_TileInfo.size();i++)
	{
		if (m_TileInfo[i]._Render == 0)
			continue;

		// 해당 타일 위치
		int iRow = i % m_Face[0];
		int iCol = i / m_Face[0];

		// 현재 스프라이트에 최대 열 최대 행
		int maxRow = int((float)_atlasImage->GetSpriteWidth() / m_PixelSize[0]);
		int maxCol = int((float)_atlasImage->GetSpriteHeight() / m_PixelSize[1]);

		// 해당 타일 UV 위치
		int _row = m_TileInfo[i]._TileIdx % maxRow;
		int _col = m_TileInfo[i]._TileIdx / maxRow;

		float _sliceX = m_PixelSize[0] / _atlasImage->GetSpriteWidth();
		float _sliceY = m_PixelSize[1] / _atlasImage->GetSpriteHeight();

		ImVec2 _stUV = ImVec2(_row * _sliceX, _col * _sliceY);
		ImVec2 _enUV = ImVec2(_row * _sliceX + _sliceX, _col * _sliceY + _sliceY);

		ImVec2 _posSt = ImVec2(origin.x + m_PixelSize[0] * iRow, origin.y + m_PixelSize[1] * iCol);
		ImVec2 _posEn = ImVec2(_posSt.x + m_PixelSize[0], _posSt.y + m_PixelSize[1]);

		draw_list->AddImage(_atlasImage.Get()->GetSRV().Get(), _posSt, _posEn, _stUV, _enUV);

		if (m_TileInfo[i]._ColliderEnabled == 1)
		{
			draw_list->AddRectFilled(_posSt, _posEn, IM_COL32(155.f, 0.f, 0.f, 100.f));
		}
	}

	// 그리드 그리기
	for (UINT _col = 0;_col < (UINT)m_Face[0];_col++)
	{
		for (UINT _row = 0;_row < (UINT)m_Face[1];_row++)
		{
			ImVec2 _posSt = ImVec2(origin.x + m_PixelSize[0] * _row, origin.y + m_PixelSize[1] * _col);
			ImVec2 _posEn = ImVec2(_posSt.x + m_PixelSize[0], _posSt.y + m_PixelSize[1]);

			draw_list->AddRect(_posSt, _posEn, IM_COL32(0, 230, 0, 255));
		}
	}
}

void TilePalette::Setting(Ptr<Sprite> _atlasImage)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 600.f));
	if (ImGui::BeginChild("Setting##TilePalette", ImVec2(-FLT_MIN, 600.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		m_prevFace[0] = (float)m_Face[0];
		m_prevFace[1] = (float)m_Face[1];

		ImGui::Text("Name"); ImGui::SameLine(150.f); ImGui::InputText("##NameInput", &m_TileName);
		ImGui::Text("Face"); ImGui::SameLine(150.f); ImGui::InputInt2("##FaceInput", m_Face);
		ImGui::Text("RenderSize"); ImGui::SameLine(150.f); ImGui::InputFloat2("##RenderSizeInput", m_RenderSize);
		ImGui::Text("PixelSize"); ImGui::SameLine(150.f); ImGui::InputFloat2("##PixelSizeInput", m_PixelSize);

		if (m_prevFace[0] != m_Face[0] || m_prevFace[1] != m_Face[1])
		{
			m_TileInfo.resize(m_Face[0] * m_Face[1]);

			for (int i = 0;i < m_TileInfo.size();i++)
			{
				if (m_TileInfo[i]._Render != 1)
					m_TileInfo[i]._Render = 0;
			}
		}
	}
	ImGui::EndChild();
}

void TilePalette::Palette(Ptr<Sprite> _atlasImage)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 600.f));
	if (ImGui::BeginChild("Palette##Palette2", ImVec2(-FLT_MIN, 600.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		if (m_PixelSize[0] == 0.f || m_PixelSize[1] == 0.f)
		{
			ImGui::EndChild();
			return;
		}

		if (ImGui::Button("Collider Box", ImVec2(m_buttonWidth, m_buttonWidth)))
		{
			m_ClickedTile._stUV = ImVec2(0.f, 0.f);
			m_ClickedTile._enUV = ImVec2(0.f, 0.f);

			m_ClickedTile._tileIdx = 0;
			m_ClickedTile._Collider = 1;
		}

		UINT _maxRow = UINT((float)_atlasImage->GetSpriteWidth() / m_PixelSize[0]);
		UINT _maxCol = UINT((float)_atlasImage->GetSpriteHeight() / m_PixelSize[1]);

		float _sliceX = m_PixelSize[0] / _atlasImage->GetSpriteWidth();
		float _sliceY = m_PixelSize[1] / _atlasImage->GetSpriteHeight();

		float _ratio = m_buttonWidth / m_PixelSize[0];

		for (UINT _col = 0; _col < _maxCol;_col++)
		{
			for (UINT _row = 0; _row < _maxRow;_row++)
			{
				ImVec2 _stUV = ImVec2(_row * _sliceX, _col * _sliceY);
				ImVec2 _enUV = ImVec2(_row * _sliceX + _sliceX, _col * _sliceY + _sliceY);

				int _idx = _col * _maxRow + _row;
				string _label = "TileButton" + std::to_string(_idx);
				if (ImGui::ImageButton(_label.c_str(), _atlasImage.Get()->GetSRV().Get(), ImVec2(m_buttonWidth, m_PixelSize[1] * _ratio), _stUV, _enUV))
				{
					m_ClickedTile._stUV = _stUV;
					m_ClickedTile._enUV = _enUV;

					m_ClickedTile._tileIdx = _col * _maxRow + _row;
					m_ClickedTile._Collider = 0;
				}
				ImGui::SameLine();
			}
			ImGui::Text("");
		}
	}
	ImGui::EndChild();
}