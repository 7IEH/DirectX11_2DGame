#include "pch.h"
#include "EHStructuredBuffer.h"

#include "EHDevice.h"

StructuredBuffer::StructuredBuffer()
	:m_ElementSize(0)
	, m_ElementCount(0)
	, m_Type(STRUCTURED_TYPE::READ_ONLY)
{
}

StructuredBuffer::~StructuredBuffer()
{
}

HRESULT StructuredBuffer::Create(UINT _elementSize, UINT _elementCount, bool _isUpdate, STRUCTURED_TYPE _type, void* _memData)
{
	m_ElementSize = _elementSize;
	m_ElementCount = _elementCount;

	m_Type = _type;

	m_StructReadBuffer = nullptr;
	m_SV = nullptr;


	assert(!(m_ElementSize % 16));

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = m_ElementCount * m_ElementSize;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = m_ElementSize;

	tDesc.CPUAccessFlags = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT _hr = E_FAIL;
	if (nullptr != _memData)
	{
		D3D11_SUBRESOURCE_DATA tData;
		tData.pSysMem = _memData;
		_hr = DEVICE->CreateBuffer(&tDesc, &tData, m_StructuredBuffer.GetAddressOf());

	}
	else
	{
		_hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_StructuredBuffer.GetAddressOf());

	}

	if (_isUpdate)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		DEVICE->CreateBuffer(&tDesc, nullptr, m_StructReadBuffer.GetAddressOf());

		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		DEVICE->CreateBuffer(&tDesc, nullptr, m_StructWriteBuffer.GetAddressOf());
	}

	if (FAILED(_hr))
	{
		return E_FAIL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc2 = {};
	tDesc2.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tDesc2.Buffer.NumElements = m_ElementCount;

	_hr = DEVICE->CreateShaderResourceView(m_StructuredBuffer.Get(), &tDesc2, m_SV.GetAddressOf());

	if (FAILED(_hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

void StructuredBuffer::UpdateData(UINT _resgisterNumber)
{
	CONTEXT->VSSetShaderResources(_resgisterNumber, 1, m_SV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_resgisterNumber, 1, m_SV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_resgisterNumber, 1, m_SV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_resgisterNumber, 1, m_SV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_resgisterNumber, 1, m_SV.GetAddressOf());
}

void StructuredBuffer::SetData(void* _memData, UINT _elementCount)
{
	assert(m_IsUpdate);

	if (_elementCount == 0)
		_elementCount = m_ElementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_StructWriteBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	::memcpy(&tSub.pData, _memData, m_ElementSize * _elementCount);
	CONTEXT->Unmap(m_StructWriteBuffer.Get(), 0);

	CONTEXT->CopyResource(m_StructuredBuffer.Get(), m_StructWriteBuffer.Get());
}

void StructuredBuffer::GetData(void* _memData, UINT _elementCount)
{
	assert(m_IsUpdate);

	if (_elementCount)
		_elementCount = m_ElementCount;

	CONTEXT->CopyResource(m_StructReadBuffer.Get(), m_StructuredBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_StructReadBuffer.Get(), 0, D3D11_MAP_READ, 0, &tSub);
	::memcpy(_memData, tSub.pData, m_ElementSize * _elementCount);
	CONTEXT->Unmap(m_StructReadBuffer.Get(), 0);
}