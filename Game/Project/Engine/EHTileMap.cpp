#include "pch.h"
#include "EHTileMap.h"

#include "EHStructuredBuffer.h"

#include "EHAssetMgr.h"
#include "EHMesh.h"
#include "EHSprite.h"

#include "EHGameObject.h"
#include "EHTransform.h"

TileMap::TileMap()
	:Renderer(RENDERER_TYPE::TILEMAP)
	, m_FaceX(2)
	, m_FaceY(2)
	, m_vTileRenderSize(Vec2(128.f, 128.f))
	, m_TileIdx(47)
	, m_TileInfoBuffer(nullptr)
{
	SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
	SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"TileMapMat"));

	m_TileInfoBuffer = new StructuredBuffer;

	SetFace(m_FaceX, m_FaceY);
}

TileMap::TileMap(const TileMap& _origin)
	:Renderer(_origin)
	, m_FaceX(_origin.m_FaceX)
	, m_FaceY(_origin.m_FaceY)
	, m_vTileRenderSize(_origin.m_vTileRenderSize)
	, m_TileAtlas(_origin.m_TileAtlas)
	, m_vTilePixelSize(_origin.m_vTilePixelSize)
	, m_vSliceSizeUV(_origin.m_vSliceSizeUV)
	, m_MaxCol(_origin.m_MaxCol)
	, m_MaxRow(_origin.m_MaxRow)
	, m_TileIdx(_origin.m_TileIdx)
	, m_vecTileInfo(_origin.m_vecTileInfo)
	, m_TileInfoBuffer(nullptr)
{
	if (nullptr != m_TileInfoBuffer)
	{
		m_TileInfoBuffer = _origin.m_TileInfoBuffer->Clone();
	}
}

TileMap::~TileMap()
{
	if (nullptr != m_TileInfoBuffer)
		delete m_TileInfoBuffer;
}

void TileMap::SetTileAtlas(Ptr<Sprite> _Atlas, Vec2 _TilePixelSize)
{
	m_TileAtlas = _Atlas;
	m_vTilePixelSize = _TilePixelSize;

	m_MaxCol = m_TileAtlas->GetSpriteWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetSpriteHeight() / (UINT)m_vTilePixelSize.y;

	m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetSpriteWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetSpriteHeight());
}

void TileMap::SetFace(UINT _FaceX, UINT _FaceY)
{
	m_FaceX = _FaceX;
	m_FaceY = _FaceY;

	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_FaceX * _FaceY);

	m_TileInfoBuffer->Create(sizeof(tTileInfo), _FaceX * _FaceY, true, STRUCTURED_TYPE::READ_ONLY);
}

void TileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx, int _render)
{
	if (nullptr == m_TileAtlas)
		return;

	UINT idx = _Row * m_FaceX + _Col;

	// �������� Ÿ�� ����
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetSpriteWidth()
		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetSpriteHeight());

	m_vecTileInfo[idx].bRender = _render;
}

void TileMap::LateUpdate()
{
	// (Ÿ�� ���� * Ÿ�� ������) �� ����� ����ó���Ѵ�.
	Vec3 vTileMapSize = Vec3(m_FaceX * m_vTileRenderSize.x, m_FaceY * m_vTileRenderSize.y, 1.f);

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->SetRelativeScale(Vec4(vTileMapSize.x, vTileMapSize.y, vTileMapSize.z, 1.f));
}

void TileMap::UpdateData()
{
}

void TileMap::Render()
{
	// ������ ��Ʋ�� �ؽ��� ����.
	GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);

	// Ÿ���� ���� ���� ����
	GetMaterial()->SetMaterialParam(INT_0, m_FaceX);
	GetMaterial()->SetMaterialParam(INT_1, m_FaceY);

	// ��Ʋ�� �̹������� Ÿ�� 1���� �ڸ��� ������(UV ����)
	GetMaterial()->SetMaterialParam(VEC2_0, m_vSliceSizeUV);

	// �� Ÿ�� ������ ����ȭ ���۷� �̵�
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), (UINT)m_vecTileInfo.size());

	// Ÿ�� ����ȭ ���۸� t20 �� ���ε�
	m_TileInfoBuffer->UpdateData(20);

	// ���� ������Ʈ
	GetMaterial()->UpdateData();

	GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->UpdateData();

	GetMesh()->Render();
}

void TileMap::Save(string _path)
{
}
