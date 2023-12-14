#include "pch.h"
#include "EHConstantBuffer.h"

#include"EHDevice.h"

void ConstantBuffer::UpdateData(int _size,int _count,void* _data)
{
	D3D11_MAPPED_SUBRESOURCE tSubData = {};
	::memset(&tSubData, 0, sizeof(tSubData));
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubData);
	::memcpy(tSubData.pData, _data, _size * _count);
	CONTEXT->Unmap(m_CB.Get(), 0);
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}
