#include "pch.h"
#include "EHGameView.h"

#include "EHRenderMgr.h"

GameView::GameView()
	:UI("Game", "##Game")
{
}

GameView::~GameView()
{
}

void GameView::Render_Update()
{
	ImVec2 _pos = ImGui::GetWindowPos();
	ImVec2 _scale = ImGui::GetWindowSize();

	ImGui::Image((void*)RenderMgr::GetInst()->GetPostProcessTexture2D().Get()->GetSRV().Get(), _scale,ImVec2(0,0),ImVec2(1,1));
}