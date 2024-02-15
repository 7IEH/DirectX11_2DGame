#include "pch.h"
#include "EHAnimationUI.h"

#include "EHAssetMgr.h"
#include "EHAnimationMgr.h"
#include "EHTimeMgr.h"
#include "EHImGUIMgr.h"

#include "EHAnimation2D.h"

AnimationUI::AnimationUI()
	:UI("AnimationUI", "##AnimationUI")
	, m_CurrentLabel("")
	, m_PrevLabel("")
	, m_LeftTop{}
	, m_SliceSize{}
	, m_frameCount(0)
	, m_fps(0.f)
	, m_Name{}
	, m_PreViewWidth(300.f)
	, m_SpriteWidth(40.f)
	, m_Acctime(0.f)
	, m_Play(FALSE)
	, m_Type(ANIMATION_UI::CREATE)
{
}

AnimationUI::~AnimationUI()
{
}

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

void AnimationUI::Render_Update()
{
	/*************
	|	Create
	*************/
	Ptr<Sprite> _AtlasImage = nullptr;

	if (m_Type == ANIMATION_UI::CREATE)
	{
		if (ImGui::Button("Create##Animation2DCreate"))
		{
			if (m_CurrentLabel != "")
			{
				_AtlasImage = AssetMgr::GetInst()->FindAsset<Sprite>(wstring(m_CurrentLabel.begin(), m_CurrentLabel.end()));
				wstring _Name = wstring(m_Name.begin(), m_Name.end());
				Vec2 _leftTop = Vec2(m_LeftTop[0], m_LeftTop[1]);
				Vec2 _SliceSize = Vec2(m_SliceSize[0], m_SliceSize[1]);

				AnimationMgr::GetInst()->CreateAnimation2D(_Name.c_str(), _AtlasImage, _leftTop, Vec2(0.f, 0.f), _SliceSize, _SliceSize, m_frameCount, m_fps);
			}
		}
	}
	else
	{
		if (ImGui::Button("Edit##Animation2DEdit"))
		{
			if (m_CurrentLabel != "")
			{
				_AtlasImage = AssetMgr::GetInst()->FindAsset<Sprite>(wstring(m_CurrentLabel.begin(), m_CurrentLabel.end()));
				wstring _Name = wstring(m_Name.begin(), m_Name.end());
				Vec2 _leftTop = Vec2(m_LeftTop[0], m_LeftTop[1]);
				Vec2 _SliceSize = Vec2(m_SliceSize[0], m_SliceSize[1]);

				// Edit로 바꿈
				AnimationMgr::GetInst()->EditAnimation2D(_Name.c_str(), _AtlasImage, _leftTop, Vec2(0.f, 0.f), _SliceSize, _SliceSize, m_frameCount, m_fps);
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit##Animation2DExit"))
	{
		ImGUIMgr::GetInst()->SetAnimationCreate(FALSE);
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
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 500.f));
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
					m_Type = ANIMATION_UI::CREATE;
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

		Cavas(_AtlasImage); ImGui::SameLine();
	}
	ImGui::EndChild();
	ImGui::SameLine();

	/*****************
	|	2. PreView
	*****************/
	ImVec2 _stUV = ImVec2(0.f, 0.f);
	ImVec2 _enUV = ImVec2(0.f, 0.f);

	// 애니메이션 플레이
	static int _frame = 0;
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

		float _width = float(_AtlasImage.Get()->GetSpriteWidth());
		float _height = float(_AtlasImage.Get()->GetSpriteHeight());

		// Ratio 보여질 이미지 크기 고정
		float _ratio = m_PreViewWidth / m_SliceSize[0];

		_stUV = ImVec2((m_LeftTop[0] + m_SliceSize[0] * _frame) / _width, m_LeftTop[1] / _height);
		_enUV = ImVec2(((m_LeftTop[0] + m_SliceSize[0] * _frame) + m_SliceSize[0]) / _width, (m_LeftTop[1] + m_SliceSize[1]) / _height);
		ImGui::SameLine(0.f); ImGui::Image(_AtlasImage.Get()->GetSRV().Get(), ImVec2(m_PreViewWidth, m_SliceSize[1] * _ratio), _stUV, _enUV);

		m_Acctime += DT;

		if (m_Acctime >= 1.f / m_fps)
		{
			m_Acctime = 0.f;
			if (m_Play)
				_frame++;
		}

		if (_frame == m_frameCount)
			_frame = 0;
	}
	ImGui::EndChild();



	/*****************
	|	3. Setting
	*****************/
	// LeftTop, SliceSize, _frameCount, _fps

	if (m_Type == ANIMATION_UI::EDIT)
	{
		Animation2D* _anim = AnimationMgr::GetInst()->Find(wstring(m_Name.begin(), m_Name.end()));
		vector<tAnimFrameInfo> _animinfo = _anim->GetFrameInfo();

		m_LeftTop[0] = _animinfo[0]._LeftTop.x;
		m_LeftTop[1] = _animinfo[0]._LeftTop.y;

		float _width = float(_AtlasImage.Get()->GetSpriteWidth());
		float _height = float(_AtlasImage.Get()->GetSpriteHeight());

		m_SliceSize[0] = _animinfo[0]._SliceSize.x * _width;
		m_SliceSize[1] = _animinfo[0]._SliceSize.y * _height;

		m_frameCount = int(_animinfo.size());

		m_fps = 1.f / _animinfo[0]._Duration;

		m_PrevLabel = m_CurrentLabel;
	}

	// Atlas 초기화시 다시 셋팅
	if (m_PrevLabel != m_CurrentLabel)
	{
		m_LeftTop[0] = 0.f;
		m_LeftTop[1] = 0.f;

		m_SliceSize[0] = 0.f;
		m_SliceSize[1] = 0.f;

		m_frameCount = 0;

		m_fps = 0.f;

		m_Name = "";

		m_PrevLabel = m_CurrentLabel;
		// Edit이였는데 바꾼건 더 이상 edit이 아니기 때문에
		// Create로 바꿈
		m_Type = ANIMATION_UI::CREATE;
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(500.f, 500.f));
	if (ImGui::BeginChild("SettingChild", ImVec2(-FLT_MIN, 300.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		ImGui::Text("LeftTop"); ImGui::SameLine(145.f); ImGui::InputFloat2("##LeftTop", m_LeftTop);
		ImGui::Text("SliceSize"); ImGui::SameLine(145.f); ImGui::InputFloat2("##SliceSize", m_SliceSize);
		ImGui::Text("FrameCount"); ImGui::SameLine(145.f); ImGui::InputInt("##FrameCount", &m_frameCount);
		ImGui::Text("FPS"); ImGui::SameLine(145.f); ImGui::InputFloat("##FPS", &m_fps);
		ImGui::Text("Animation Name"); ImGui::SameLine(145.f); ImGui::InputText("##AnimationName", &m_Name);
	}
	ImGui::EndChild();
	ImGui::SameLine();

	/*********************
	|	4. Atlas Preview
	*********************/
	// 애니메이션 스프라이트
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(50.f * m_frameCount, 300.f));
	if (ImGui::BeginChild("SpritePreviewChild", ImVec2(50.f * m_frameCount, 300.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX))
	{
		float _width = float(_AtlasImage.Get()->GetSpriteWidth());
		float _height = float(_AtlasImage.Get()->GetSpriteHeight());

		float _ratio = m_SpriteWidth / m_SliceSize[0];
		for (int i = 0;i < m_frameCount;i++)
		{
			_stUV = ImVec2((m_LeftTop[0] + m_SliceSize[0] * i) / _width, m_LeftTop[1] / _height);
			_enUV = ImVec2(((m_LeftTop[0] + m_SliceSize[0] * i) + m_SliceSize[0]) / _width, (m_LeftTop[1] + m_SliceSize[1]) / _height);

			if (_frame == i)
				ImGui::ImageButton(_AtlasImage.Get()->GetSRV().Get(), ImVec2(m_SpriteWidth, m_SliceSize[1] * _ratio), _stUV, _enUV, -1, ImVec4(0, 0, 0, 0), ImVec4(1.f, 0.f, 0.f, 1.f));
			else
				ImGui::ImageButton(_AtlasImage.Get()->GetSRV().Get(), ImVec2(m_SpriteWidth, m_SliceSize[1] * _ratio), _stUV, _enUV);
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
}

void AnimationUI::Cavas(Ptr<Sprite> _AtlasImage)
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

	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
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