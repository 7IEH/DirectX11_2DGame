#include "pch.h"
#include "EHConstantBuffer.h"

#include"EHDevice.h"

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
}
