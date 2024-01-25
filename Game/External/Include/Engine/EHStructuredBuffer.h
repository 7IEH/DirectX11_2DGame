#pragma once

class StructuredBuffer
{
private:
	ComPtr<ID3D11Buffer>					m_StructuredBuffer;

	ComPtr<ID3D11Buffer>					m_StructWriteBuffer;
	ComPtr<ID3D11Buffer>					m_StructReadBuffer;

	ComPtr<ID3D11ShaderResourceView>		m_SV;
	ComPtr<ID3D11UnorderedAccessView>		m_UAV;

	UINT									m_ElementSize;
	UINT									m_ElementCount;

	STRUCTURED_TYPE							m_Type;
	bool									m_IsUpdate;

public:
	HRESULT									Create(UINT _elementSize, UINT _elementCount, bool _isUpdate, 
													STRUCTURED_TYPE _type = STRUCTURED_TYPE::READ_ONLY, void* _memData = nullptr);

	void									UpdateData(UINT	_resgisterNumber);

	void									SetData(void* _memData, UINT _elementCount = 0);
	void									GetData(void* _memData, UINT _elementCount = 0);

public:
	StructuredBuffer();
	~StructuredBuffer();
};