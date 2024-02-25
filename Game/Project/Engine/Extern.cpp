#include "pch.h"
#include "Extern.h"

transform e_MatrixData = {};

tGlobalData e_Global = {};

wstring e_Font = {};

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