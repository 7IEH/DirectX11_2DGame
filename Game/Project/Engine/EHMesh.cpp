#include "pch.h"
#include "EHMesh.h"

#include "EHDevice.h"

Mesh::Mesh()
	:Asset(ASSET_TYPE::MESH)
	,m_IndexCount(0)
	,m_VB(nullptr)
	,m_IB(nullptr)
	,m_VertexInfo(nullptr)
	,m_IndexInfo(nullptr)
{
}

Mesh::~Mesh()
{
}

void Mesh::Create(void* _VBData, UINT _VertexCount, void* _IBData, UINT _IndexCount)
{
	m_IndexCount = _IndexCount;

	CreateBuffer(BUFFER_TYPE::VERTEX, _VertexCount, _VBData);
	CreateBuffer(BUFFER_TYPE::INDEX, _IndexCount, _IBData);
	
	// 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VertexInfo = new vtx[_VertexCount];
	m_IndexInfo = new UINT[_IndexCount];

	memcpy(m_VertexInfo, _VBData, sizeof(vtx) * _VertexCount);
	memcpy(m_IndexInfo, _IBData, sizeof(UINT) * _IndexCount);
}

void Mesh::UpdateData()
{
	SetBuffer(BUFFER_TYPE::VERTEX);
	SetBuffer(BUFFER_TYPE::INDEX);
}

void Mesh::Render()
{
	DrawIndexed();
}

void Mesh::CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, void* _data)
{
	// Buffer Initial Data Setting
	D3D11_SUBRESOURCE_DATA tsubData = {};
	::memset(&tsubData, 0, sizeof(tsubData));
	tsubData.pSysMem = _data;

	// Buffer Setting
	D3D11_BUFFER_DESC _bDesc = {};
	::memset(&_bDesc, 0, sizeof(_bDesc));
	_bDesc.MiscFlags = 0;
	_bDesc.CPUAccessFlags = 0;
	_bDesc.Usage = D3D11_USAGE_DEFAULT;

	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		_bDesc.StructureByteStride = sizeof(vtx);
		_bDesc.ByteWidth = sizeof(vtx) * _count;
		_bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		DEVICE->CreateBuffer(&_bDesc, &tsubData, &m_VB);
	}
	break;
	case BUFFER_TYPE::INDEX:
	{
		_bDesc.StructureByteStride = sizeof(UINT);
		_bDesc.ByteWidth = sizeof(UINT) * _count;
		_bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		DEVICE->CreateBuffer(&_bDesc, &tsubData, &m_IB);
	}
	break;
	default:
		break;
	}
}

void Mesh::DrawIndexed()
{
	CONTEXT->DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::SetBuffer(BUFFER_TYPE _bufferType)
{
	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		UINT iStride = sizeof(vtx);
		UINT iOffset = 0;
		CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	}
		break;
	case BUFFER_TYPE::INDEX:
	{
		CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
		break;
	default:
		break;
	}
}