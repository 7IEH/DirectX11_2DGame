#include "pch.h"
#include "EHConstantBuffer.h"

#include"EHDevice.h"

ConstantBuffer::ConstantBuffer()
	:m_CB(nullptr)
	,m_Type(CONSTANT_TYPE::TRANSFORM)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::SetData(void* _data, int _size, int _count)
{
	if (_count == 0)
	{
		HandleError(Device::GetInst()->GetHwnd(), L"ConstantBuffer SetData Count Zero Error!", 2);
		return;
	}

	D3D11_MAPPED_SUBRESOURCE tSubData = {};
	::memset(&tSubData, 0, sizeof(tSubData));
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubData);
	::memcpy(tSubData.pData, _data, _size * _count);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void ConstantBuffer::UpdateData()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}