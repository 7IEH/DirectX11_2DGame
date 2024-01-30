#pragma once
#include "EHAsset.h"

class Mesh
	:public Asset
{
private:
	ComPtr<ID3D11Buffer>		m_VB;
	ComPtr<ID3D11Buffer>		m_IB;

	UINT						m_IndexCount;

	void*						m_VertexInfo;
	void*						m_IndexInfo;

public:
	Mesh();
	virtual ~Mesh();

public:
	void Create(void* _VBData, UINT _VertexCount, void* _IBData, UINT _IndexCount);
	void UpdateData();

public:
	void Render();
	void Render_Instancing(UINT _particleCount);

private:
	// Re-Packaging ( DX12 )
	void CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, void* _data);
	void DrawIndexed();
	void SetBuffer(BUFFER_TYPE _bufferType);
};

