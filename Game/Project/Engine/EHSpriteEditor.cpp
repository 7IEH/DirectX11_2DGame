#include "pch.h"
#include "EHSpriteEditor.h"

#include "EHAssetMgr.h"

SpriteEditor::SpriteEditor()
	:UI("SpriteEditor","##SpriteEditor")
{
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Render_Update()
{
	ID3D11ShaderResourceView* view = AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerIdleFront").Get()->GetSRV().Get();
	float _width = AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerIdleFront").Get()->GetSpriteWidth();
	float _height = AssetMgr::GetInst()->FindAsset<Sprite>(L"PlayerIdleFront").Get()->GetSpriteHeight();

	ImGui::Image(view,ImVec2(_width,_height));
}