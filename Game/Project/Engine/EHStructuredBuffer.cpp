#include "pch.h"
#include "EHStructuredBuffer.h"

#include "EHDevice.h"

StructuredBuffer::StructuredBuffer()
	: m_ElementSize(0)
	, m_ElementCount(0)
	, m_Type(STRUCTURED_TYPE::READ_ONLY)
	, m_IsUpdate(FALSE)
	, m_RegentSRV(0)
	, m_RegentUAV(0)
{
}

StructuredBuffer::StructuredBuffer(const StructuredBuffer& _origin)
	:Entity(_origin)
	, m_StructuredBuffer(nullptr)
	, m_StructWriteBuffer(nullptr)
	, m_StructReadBuffer(nullptr)
	, m_SV(nullptr)
	, m_UAV(nullptr)
	, m_ElementSize(_origin.m_ElementSize)
	, m_ElementCount(_origin.m_ElementCount)
	, m_RegentSRV(0)
	, m_RegentUAV(0)
	, m_Type(_origin.m_Type)
	, m_IsUpdate(_origin.m_IsUpdate)
{
	Create(m_ElementSize, m_ElementCount, m_IsUpdate , m_Type);
}

StructuredBuffer::~StructuredBuffer()
{
}

HRESULT StructuredBuffer::Create(UINT _elementSize, UINT _elementCount, bool _isUpdate, STRUCTURED_TYPE _type, void* _memData)
{
	m_ElementSize = _elementSize;
	m_ElementCount = _elementCount;
	m_Type = _type;
	m_IsUpdate = _isUpdate;

	m_StructuredBuffer = nullptr;
	m_StructWriteBuffer = nullptr;
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

	if (STRUCTURED_TYPE::READ_WRITE == m_Type)
	{
		tDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

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
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
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

	if (STRUCTURED_TYPE::READ_WRITE == m_Type)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_ElementCount;

		_hr = DEVICE->CreateUnorderedAccessView(m_StructuredBuffer.Get(), &UAVDesc, m_UAV.GetAddressOf());
		if (FAILED(_hr)) return E_FAIL;
	}

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

HRESULT StructuredBuffer::UpdateData_CS_SRV(UINT _registerNum)
{
	if (nullptr == m_SV.Get())
		return E_FAIL;

	m_RegentSRV = _registerNum;

	CONTEXT->CSSetShaderResources(_registerNum, 1, m_SV.GetAddressOf());

	return S_OK;
}

HRESULT StructuredBuffer::UpdateData_CS_UAV(UINT _registerNum)
{
	if (nullptr == m_UAV.Get())
		return E_FAIL;

	m_RegentUAV = _registerNum;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, m_UAV.GetAddressOf(), &i);

	return S_OK;
}

void StructuredBuffer::Clear(UINT _registerNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_registerNum, 1, &pSRV);
}

void StructuredBuffer::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* _SRV = nullptr;

	CONTEXT->CSSetShaderResources(m_RegentSRV, 1, &_SRV);
}

void StructuredBuffer::Clear_CS_UAV()
{
	ID3D11UnorderedAccessView* _UAV = nullptr;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RegentUAV, 1, &_UAV, &i);
}

void StructuredBuffer::SetData(void* _memData, UINT _elementCount)
{
	if (_memData == nullptr)
		return;

	assert(m_IsUpdate);

	if (_elementCount == 0)
		_elementCount = m_ElementCount;
	else
		m_iCurElementCount = _elementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_StructWriteBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	::memcpy(tSub.pData, _memData, m_ElementSize * _elementCount);
	CONTEXT->Unmap(m_StructWriteBuffer.Get(), 0);

	CONTEXT->CopyResource(m_StructuredBuffer.Get(), m_StructWriteBuffer.Get());
}

void StructuredBuffer::GetData(void* _memData, UINT _elementCount)
{
	assert(m_IsUpdate);

	if (!_elementCount)
		_elementCount = m_ElementCount;

	CONTEXT->CopyResource(m_StructReadBuffer.Get(), m_StructuredBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_StructReadBuffer.Get(), 0, D3D11_MAP_READ, 0, &tSub);
	::memcpy(_memData, tSub.pData, m_ElementSize * _elementCount);
	CONTEXT->Unmap(m_StructReadBuffer.Get(), 0);
}