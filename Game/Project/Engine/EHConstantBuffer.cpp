#include "pch.h"
#include "EHConstantBuffer.h"

#include"EHDevice.h"

ConstantBuffer::ConstantBuffer()
	:m_Desc{}
	, m_Type(CONSTANT_TYPE::TRANSFORM)
	, m_ElementSize(0)
	, m_ElementCount(0)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

HRESULT ConstantBuffer::Create(UINT _elementSize, UINT _elementCount, CONSTANT_TYPE _type)
{
	m_ElementSize = _elementSize;
	m_ElementCount = _elementCount;
	m_Type = _type;

	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;


	HRESULT _hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf());

	if (FAILED(_hr))
	{
		HandleError(MAIN_HWND, L"ConstantBuffer Create Failed!", 0);
		return E_FAIL;
	}

	return S_OK;
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
	::memcpy(tSubData.pData, _data, m_ElementSize * _count);
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

VOID ConstantBuffer::UpdateData_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}