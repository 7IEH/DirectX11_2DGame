#include "pch.h"
#include "Extern.h"

#include "EHEngine.h"

transform e_MatrixData = {};

tGlobalData e_Global = {};

wstring e_Font = {};

Vec2 e_vRatio = Vec2(Engine::GetInst()->GetResolution().x / 1600.f, Engine::GetInst()->GetResolution().y / 900.f);

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]
{
	"MESH",
	"SHADER",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
	"MATERIAL",
	"TEXTURE",
	"SPRITE",
	"SOUND"
};