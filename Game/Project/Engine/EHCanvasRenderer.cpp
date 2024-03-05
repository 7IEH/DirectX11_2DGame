#include "pch.h"
#include "EHCanvasRenderer.h"

#include "EHText.h"
#include "EHGameObject.h"

#include "EHAssetMgr.h"

CanvasRenderer::CanvasRenderer()
	:Renderer(RENDERER_TYPE::CANVAS)
{
	SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"BackGroundMaterial"));
}

CanvasRenderer::~CanvasRenderer()
{
}

void CanvasRenderer::Render()
{
	Text* _text = GetOwner()->GetComponent<Text>(COMPONENT_TYPE::TEXT);
	if (nullptr != _text)
		_text->Render();
}

void CanvasRenderer::Save(string _path)
{
	std::ofstream _file(_path, std::fstream::out | std::fstream::app);
	
	_file << "CANVASRENDERER\n";

	_file.close();
}

void CanvasRenderer::Load(std::wifstream* _file)
{

}
