#include "pch.h"
#include "unAnimation2DUI.h"

#include "EHAssetMgr.h"
#include "EHAnimationMgr.h"
#include "EHTimeMgr.h"
#include "EHImGUIMgr.h"

#include "EHAnimation2D.h"

/*********************************************
||--------------|		|----------------|
||				|		|				 |
||  Atlas Image |		|	PreView	     |
||				|		|				 |
||______________|		|________________|
|
||--------------|		|----------------|
||				|		|				 |
||	 Setting	|		|  Atlas Preview |
||				|		|				 |
||______________|		|________________|
*********************************************/


unAnimation2DUI::unAnimation2DUI()
	:UI("AnimationUI", "##unAnimationUI")
	, m_dirY{ -1,1,0,0 }
	, m_dirX{ 0,0,-1,1 }
	, m_AnimationInfo{}
	, m_PreViewWidth(300.f)
{
}

unAnimation2DUI::~unAnimation2DUI()
{
}

void unAnimation2DUI::Render_Update()
{
	/*************
	|	Create
	*************/
	Ptr<Sprite> _AtlasImage = nullptr;

	if (ImGui::Button("Create##unAnimation2DCreate"))
	{
		if (m_CurrentLabel != "")
		{
			_AtlasImage = AssetMgr::GetInst()->FindAsset<Sprite>(wstring(m_CurrentLabel.begin(), m_CurrentLabel.end()));
			wstring _Name = wstring(m_Name.begin(), m_Name.end());
			AnimationMgr::GetInst()->PushAnimation2D(_Name.c_str(), _AtlasImage, m_AnimationInfo);
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Exit##unAnimation2DExit"))
	{
		ImGUIMgr::GetInst()->SetunAnimationCreateUI(FALSE);
	}

	// 현재 AssetMgr가 가지고 있는 Sprite 가져옴
	map<wstring, Ptr<Asset>> _spriteInfo = AssetMgr::GetInst()->GetSprite();
	map<wstring, Ptr<Asset>>::iterator iter = _spriteInfo.begin();

	iter = _spriteInfo.begin();
	bool _flag = false;

	// 현재 골라진 atlasImage

	/*********************
	|	1. Atlas Image
	*********************/
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(800.f, 500.f));
	if (ImGui::BeginChild("AtlasImageChild", ImVec2(-FLT_MIN, 500.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		ImGui::Text("Atlas"); ImGui::SameLine();
		if (ImGui::BeginCombo("##AnimationUIAtlas", m_CurrentLabel.c_str()))
		{
			for (;iter != _spriteInfo.end();iter++)
			{
				bool is_selected = (m_CurrentLabel == string(iter->first.begin(), iter->first.end()));
				string _comboChild = string(iter->first.begin(), iter->first.end());
				if (ImGui::Selectable(_comboChild.c_str(), _flag))
				{
					m_CurrentLabel = _comboChild;
					m_Frame = 0;
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

		_AtlasImage = dynamic_cast<Sprite*>(iter->second.Get());

		/*******************
		|	Canvas
		*******************/
		Canvas(_AtlasImage);
	}
	ImGui::EndChild();
	ImGui::SameLine();

	/*****************
	|	2. PreView
	*****************/
	Preview(_AtlasImage);

	/*****************
	|	3. Setting
	*****************/
	// LeftTop, SliceSize, _frameCount, _fps
	Setting(_AtlasImage);

	/*********************
	|	4. Atlas Preview
	*********************/
	// 애니메이션 스프라이트
	AtlasPreview(_AtlasImage);
}

void unAnimation2DUI::Canvas(Ptr<Sprite> _AtlasImage)
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
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


	draw_list->AddImage(_AtlasImage.Get()->GetSRV().Get(), ImVec2(origin.x, origin.y), ImVec2(origin.x + _AtlasImage->GetSpriteWidth() * 4, origin.y + _AtlasImage->GetSpriteHeight() * 4));

	ScratchImage* _image = _AtlasImage.Get()->GetScratchImage();

	uint8_t* picData = _image->GetPixels();

	uint8_t* pixel = picData;
	for (int i = 0; i < _image->GetImages()->height; ++i) {
		for (int j = 0; j < _image->GetImages()->width; ++j, pixel += 4) {
			atlasPixel[i][j].r = pixel[0];
			atlasPixel[i][j].g = pixel[1];
			atlasPixel[i][j].b = pixel[2];
			atlasPixel[i][j].a = pixel[3];
		}
	}

	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	int minX = 0x3f3f3f3f;
	int minY = 0x3f3f3f3f;
	int maxX = -300;
	int	maxY = -300;
	queue<std::pair<int, int>>q;
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && is_hovered)
	{
		m_RectForVisible[0] = ImVec2(0.f, 0.f);
		m_RectForVisible[1] = ImVec2(0.f, 0.f);

		int mouse_pos_x = mouse_pos_in_canvas.x / 4.f;
		int mouse_pos_y = mouse_pos_in_canvas.y / 4.f;

		q.push(make_pair(mouse_pos_y, mouse_pos_x));
		visited[(int)mouse_pos_y][(int)mouse_pos_x] = true;

		while (!q.empty())
		{
			auto now = q.front(); q.pop();

			for (int dir = 0;dir < 4;dir++)
			{
				int ny = now.first + m_dirY[dir];
				int nx = now.second + m_dirX[dir];

				if (ny < 0 || nx < 0)continue;
				if (visited[ny][nx])continue;
				if (atlasPixel[ny][nx].a == 0.f)continue;

				if (minX > nx)
				{
					minX = nx;
				}

				if (minY > ny)
				{
					minY = ny;
				}

				if (maxX < nx)
				{
					maxX = nx;
				}

				if (maxY < ny)
				{
					maxY = ny;
				}

				q.push({ ny,nx });
				visited[ny][nx] = true;
			}
		}

		adding_line = true;

		m_RectForVisible[0] = ImVec2((float)minX - 1.f, (float)minY - 1.f);
		m_RectForVisible[1] = ImVec2((float)maxX + 1.5f, (float)maxY + 1.5f);

		m_LeftTop[0] = m_RectForVisible[0].x;
		m_LeftTop[1] = m_RectForVisible[0].y;

		m_SliceSize[0] = m_RectForVisible[1].x - m_RectForVisible[0].x;
		m_SliceSize[1] = m_RectForVisible[1].y - m_RectForVisible[0].y;
	}

	draw_list->AddRect(ImVec2(origin.x + m_RectForVisible[0].x * 4.f, origin.y + m_RectForVisible[0].y * 4.f),
		ImVec2(origin.x + m_RectForVisible[1].x * 4.f, origin.y + m_RectForVisible[1].y * 4.f), IM_COL32(0, 255, 0, 255));

	if (adding_line)
	{
		for (int y = 0;y < 5000;y++)
		{
			for (int x = 0;x < 5000;x++)
			{
				visited[y][x] = false;
			}
		}
		adding_line = false;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}
	for (int n = 0; n < points.Size; n += 2)
		draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
	draw_list->PopClipRect();
}

void unAnimation2DUI::Preview(Ptr<Sprite> _AtlasImage)
{
	ImVec2 _stUV = ImVec2(0.f, 0.f);
	ImVec2 _enUV = ImVec2(0.f, 0.f);

	// 애니메이션 플레이
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 500.f));
	if (ImGui::BeginChild("PreViewChild", ImVec2(-FLT_MIN, 500.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		if (ImGui::Button("Play"))
		{
			m_Play = TRUE;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			m_Play = FALSE;
		}

		ImGui::Separator();

		float _width = _AtlasImage.Get()->GetSpriteWidth();
		float _height = _AtlasImage.Get()->GetSpriteHeight();

		// Ratio 보여질 이미지 크기 고정
		if (m_AnimationInfo.size() == 0)
		{
			ImGui::EndChild();
			return;
		}

		ImVec2 _leftTop = ImVec2(m_AnimationInfo[m_Frame]._LeftTop.x, m_AnimationInfo[m_Frame]._LeftTop.y);
		ImVec2 _offSet = ImVec2(m_AnimationInfo[m_Frame]._Offset.x, m_AnimationInfo[m_Frame]._Offset.y);
		ImVec2 _sliceSize = ImVec2(m_AnimationInfo[m_Frame]._SliceSize.x, m_AnimationInfo[m_Frame]._SliceSize.y);

		_stUV = ImVec2(_leftTop.x + _offSet.x, _leftTop.y + _offSet.y);
		_enUV = ImVec2(_leftTop.x + _offSet.x + _sliceSize.x, _leftTop.y + _offSet.y + _sliceSize.y);
		ImGui::Image(_AtlasImage.Get()->GetSRV().Get(), ImVec2(((_enUV.x - _stUV.x) * _AtlasImage->GetSpriteWidth()) * 4.f,
			((_enUV.y - _stUV.y) * _AtlasImage->GetSpriteHeight()) * 4.f), _stUV, _enUV);

		m_Acctime += DT;

		if (m_Acctime >= m_AnimationInfo[m_Frame]._Duration)
		{
			m_Acctime = 0.f;
			if (m_Play)
				m_Frame++;
		}

		if (m_Frame >= m_AnimationInfo.size())
			m_Frame = 0;


		ImGui::SeparatorText("Frame Info");

		ImGui::Text("LeftTop"); ImGui::SameLine(145.f); ImGui::InputFloat2("##LeftTop2", m_EditLeftTop);
		ImGui::Text("SliceSize"); ImGui::SameLine(145.f); ImGui::InputFloat2("##SliceSize2", m_EditSliceSize);
		ImGui::Text("Offset"); ImGui::SameLine(145.f); ImGui::InputFloat2("##Offset", m_EditOffset);
		ImGui::Text("FPS"); ImGui::SameLine(145.f); ImGui::InputFloat("##FPS2", &m_Editfps);

		ImGui::Text("SwapIndex1"); ImGui::SameLine(145.f); ImGui::InputInt("##prevSwap", &m_PrevSwap);
		ImGui::Text("SwapIndex2"); ImGui::SameLine(145.f); ImGui::InputInt("##nxtSwap", &m_NxtSwap);

		ImGui::Separator();

		if (ImGui::Button("Edit##EditUnanimation2D"))
		{
			float _width = _AtlasImage->GetSpriteWidth();
			float _height = _AtlasImage->GetSpriteHeight();

			m_AnimationInfo[m_Frame]._LeftTop.x = m_EditLeftTop[0] / _width;
			m_AnimationInfo[m_Frame]._LeftTop.y = m_EditLeftTop[1] / _height;

			m_AnimationInfo[m_Frame]._SliceSize.x = m_EditSliceSize[0] / _width;
			m_AnimationInfo[m_Frame]._SliceSize.y = m_EditSliceSize[1] / _height;

			m_AnimationInfo[m_Frame]._Offset.x = m_EditOffset[0] / _width;
			m_AnimationInfo[m_Frame]._Offset.y = m_EditOffset[1] / _height;

			m_AnimationInfo[m_Frame]._Duration = 1.f / m_Editfps;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete##DeleteUnAnimation2D"))
		{
			vector<tAnimFrameInfo>::iterator iter = m_AnimationInfo.begin();
			for (int i = 0;i < m_Frame;i++)
			{
				iter++;
			}
			iter = m_AnimationInfo.erase(iter);
		}
		ImGui::SameLine();
		if (ImGui::Button("Swap##SwapUnAnimation2D"))
		{
			tAnimFrameInfo _swap = {};
			_swap = m_AnimationInfo[m_PrevSwap];
			m_AnimationInfo[m_PrevSwap] = m_AnimationInfo[m_NxtSwap];
			m_AnimationInfo[m_NxtSwap] = _swap;
		}
	}
	ImGui::EndChild();
}

void unAnimation2DUI::Setting(Ptr<Sprite> _AtlasImage)
{
	// Atlas 초기화시 다시 셋팅
	if (m_PrevLabel != m_CurrentLabel)
	{
		m_Name = "";

		m_LeftTop[0] = 0.f;
		m_LeftTop[1] = 0.f;

		m_SliceSize[0] = 0.f;
		m_SliceSize[1] = 0.f;

		m_fps = 0.f;

		m_PrevLabel = m_CurrentLabel;
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 500.f));
	if (ImGui::BeginChild("SettingChild", ImVec2(-FLT_MIN, 300.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		ImGui::Text("Animation Name"); ImGui::SameLine(145.f); ImGui::InputText("##AnimationName2", &m_Name);
		ImGui::Text("LeftTop"); ImGui::SameLine(145.f); ImGui::InputFloat2("##LeftTop2", m_LeftTop);
		ImGui::Text("SliceSize"); ImGui::SameLine(145.f); ImGui::InputFloat2("##SliceSize2", m_SliceSize);
		ImGui::Text("Offset"); ImGui::SameLine(145.f); ImGui::InputFloat2("##Offset", m_Offset);
		ImGui::Text("FPS"); ImGui::SameLine(145.f); ImGui::InputFloat("##FPS2", &m_fps);

		m_RectForVisible[0].x = m_LeftTop[0] + m_Offset[0];
		m_RectForVisible[0].y = m_LeftTop[1] + m_Offset[1];

		m_RectForVisible[1].x = m_LeftTop[0] + m_Offset[0] + m_SliceSize[0];
		m_RectForVisible[1].y = m_LeftTop[1] + m_Offset[1] + m_SliceSize[1];
	}

	if (ImGui::Button("Frame Create"))
	{
		float _width = _AtlasImage->GetSpriteWidth();
		float _height = _AtlasImage->GetSpriteHeight();
		tAnimFrameInfo _info = {};
		_info._LeftTop.x = m_LeftTop[0] / _width;
		_info._LeftTop.y = m_LeftTop[1] / _height;
		_info._Offset.x = m_Offset[0] / _width;
		_info._Offset.y = m_Offset[1] / _height;
		_info._SliceSize.x = m_SliceSize[0] / _width;
		_info._SliceSize.y = m_SliceSize[1] / _height;

		_info._Duration = 1.f / m_fps;
		_info._BackGround.x = m_SliceSize[0] / _width;
		_info._BackGround.y = m_SliceSize[1] / _height;

		m_AnimationInfo.push_back(_info);
	}

	ImGui::EndChild();
	ImGui::SameLine();
}

void unAnimation2DUI::AtlasPreview(Ptr<Sprite> _AtlasImage)
{
	ImVec2 _stUV = ImVec2(0.f, 0.f);
	ImVec2 _enUV = ImVec2(0.f, 0.f);

	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 300.f));
	if (ImGui::BeginChild("SpritePreviewChild", ImVec2(1000.f, 300.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		float _width = _AtlasImage.Get()->GetSpriteWidth();
		float _height = _AtlasImage.Get()->GetSpriteHeight();

		float _ratio = m_SpriteWidth / m_SliceSize[0];
		for (int i = 0;i < m_AnimationInfo.size();i++)
		{
			ImVec2 _leftTop = ImVec2(m_AnimationInfo[i]._LeftTop.x, m_AnimationInfo[i]._LeftTop.y);
			ImVec2 _sliceSize = ImVec2(m_AnimationInfo[i]._SliceSize.x, m_AnimationInfo[i]._SliceSize.y);
			ImVec2 _offset = ImVec2(m_AnimationInfo[i]._Offset.x, m_AnimationInfo[i]._Offset.y);

			_stUV = ImVec2(_leftTop.x + _offset.x, _leftTop.y + _offset.y);
			_enUV = ImVec2(_leftTop.x + _offset.x + _sliceSize.x, _leftTop.y + _offset.y + _sliceSize.y);

			if (m_Frame == i)
			{
				ImGui::ImageButton(_AtlasImage.Get()->GetSRV().Get(), ImVec2(50.f, 30.f), _stUV, _enUV, -1, ImVec4(0, 0, 0, 0), ImVec4(1.f, 0.f, 0.f, 1.f));
			}
			else
			{
				ImGui::ImageButton(_AtlasImage.Get()->GetSRV().Get(), ImVec2(50.f, 30.f), _stUV, _enUV);
			}
			ImGui::SameLine();
		}

		ImGui::Text("");

		for (int i = 0;i < m_AnimationInfo.size();i++)
		{
			ImVec2 _leftTop = ImVec2(m_AnimationInfo[i]._LeftTop.x, m_AnimationInfo[i]._LeftTop.y);
			ImVec2 _sliceSize = ImVec2(m_AnimationInfo[i]._SliceSize.x, m_AnimationInfo[i]._SliceSize.y);
			ImVec2 _offset = ImVec2(m_AnimationInfo[i]._Offset.x, m_AnimationInfo[i]._Offset.y);

			_stUV = ImVec2(_leftTop.x + _offset.x, _leftTop.y + _offset.y);
			_enUV = ImVec2(_leftTop.x + _offset.x + _sliceSize.x, _leftTop.y + _offset.y + _sliceSize.y);

			string s = "F";
			s += std::to_string(i);
			if (ImGui::Button(s.c_str(), ImVec2(50.f, 30.f)))
			{
				m_Frame = i;
				m_Play = FALSE;

				m_EditLeftTop[0] = _leftTop.x * _width;
				m_EditLeftTop[1] = _leftTop.y * _height;

				m_EditSliceSize[0] = _sliceSize.x * _width;
				m_EditSliceSize[1] = _sliceSize.y * _height;

				m_EditOffset[0] = _offset.x * _width;
				m_EditOffset[0] = _offset.y * _height;

				m_Editfps = 1.f / m_AnimationInfo[i]._Duration;
			}
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
}