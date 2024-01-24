#include "pch.h"
#include "EHTestUI.h"

#include "EHAssetMgr.h"

struct pixel
{
	float r;
	float g;
	float b;
	float a;
};

pixel rgb[1000][1000];
bool visited[1000][1000];

int dirY[4] = { 0,0,-1,1 };
int dirX[4] = { 1,-1,0,0 };

TestUI::TestUI()
	:UI("TestUI", "##TestUI")
	, m_Width(500.f)
{
}

TestUI::~TestUI()
{
}

void TestUI::Render_Update()
{
	
}