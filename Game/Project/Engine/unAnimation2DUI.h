#pragma once
#include "EHUI.h"
#include "EHAssetMgr.h"
#include "EHAnimation2D.h"

struct PixelColor
{
	float r;
	float g;
	float b;
	float a;
};

class unAnimation2DUI :
	public UI
{
private:
	vector<tAnimFrameInfo> m_AnimationInfo;

	// 이차원 배열 동적할당 받기
	PixelColor atlasPixel[5000][5000];
	bool visited[5000][5000];

	ImVec2 m_RectForVisible[2];

	string m_CurrentLabel;
	string m_PrevLabel;

	float m_LeftTop[2];
	float m_SliceSize[2];
	float m_Offset[2];
	float m_fps;
	string m_Name;

	// Preview
	float m_PreViewWidth;
	bool m_Play;

	// Sprite Preview
	float m_Acctime;
	float m_SpriteWidth;

	int	  m_Frame;

	int	  m_dirY[4];
	int	  m_dirX[4];

	float m_EditLeftTop[2];
	float m_EditSliceSize[2];
	float m_EditOffset[2];
	float m_Editfps;

	int m_PrevSwap;
	int	m_NxtSwap;

public:
	virtual void Render_Update() override;

private:
	void Canvas(Ptr<Sprite> _AtlasImage);
	void Preview(Ptr<Sprite> _AtlasImage);
	void Setting(Ptr<Sprite> _AtlasImage);
	void AtlasPreview(Ptr<Sprite> _AtlasImage);

public:
	unAnimation2DUI();
	virtual ~unAnimation2DUI();
};

