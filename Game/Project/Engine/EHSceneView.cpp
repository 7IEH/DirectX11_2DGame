#include "pch.h"
#include "EHSceneView.h"

#include "EHRenderMgr.h"

SceneView::SceneView()
	:UI("Scene","##Scene")
{
}

SceneView::~SceneView()
{
}

void SceneView::Render_Update()
{
	ImVec2 _pos = ImGui::GetWindowPos();
	ImVec2 _scale = ImGui::GetWindowSize();

	ImGui::Image((void*)RenderMgr::GetInst()->GetPostProcessTexture2D().Get()->GetSRV().Get(), _scale, ImVec2(0.5, 0), ImVec2(1, 1));
}