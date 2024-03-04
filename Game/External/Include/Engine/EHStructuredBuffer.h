#pragma once

#include "EHEntity.h"

class StructuredBuffer
	:public Entity
{
private:
	ComPtr<ID3D11Buffer>					m_StructuredBuffer;

	ComPtr<ID3D11Buffer>					m_StructWriteBuffer;
	ComPtr<ID3D11Buffer>					m_StructReadBuffer;

	ComPtr<ID3D11ShaderResourceView>		m_SV;
	ComPtr<ID3D11UnorderedAccessView>		m_UAV;

	UINT									m_ElementSize;
	UINT									m_ElementCount;
	
	UINT									m_iCurElementCount;

	UINT									m_RegentSRV;
	UINT									m_RegentUAV;

	STRUCTURED_TYPE							m_Type;
	bool									m_IsUpdate;

public:
	HRESULT									Create(UINT _elementSize, UINT _elementCount, bool _isUpdate, 
													STRUCTURED_TYPE _type = STRUCTURED_TYPE::READ_ONLY, void* _memData = nullptr);

	void									UpdateData(UINT	_resgisterNumber);
	HRESULT									UpdateData_CS_SRV(UINT _RegisterNum);
	HRESULT									UpdateData_CS_UAV(UINT _RegisterNum);

	void									Clear(UINT	_registerNum);
	void									Clear_CS_SRV();
	void									Clear_CS_UAV();

	void									SetData(void* _memData, UINT _elementCount = 0);
	void									GetData(void* _memData, UINT _elementCount = 0);

	UINT									GetElementSize() { return m_ElementSize; }
	UINT									GetElementCount() { return m_ElementCount; }
	
	UINT									GetCurElementCount() { return m_iCurElementCount; }

	CLONE(StructuredBuffer)
public:
	StructuredBuffer();
	StructuredBuffer(const StructuredBuffer& _origin);
	~StructuredBuffer();
};