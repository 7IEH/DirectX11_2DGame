#pragma once
#include "EHEntity.h"
class ConstantBuffer
	:public Entity
{
private:
	ComPtr<ID3D11Buffer>	m_CB;
	D3D11_BUFFER_DESC		m_Desc;

	CONSTANT_TYPE			m_Type;

	UINT					m_ElementSize;
	UINT					m_ElementCount;

public:
	ID3D11Buffer*	GetBuffer() { return m_CB.Get(); }
	ID3D11Buffer**	GetBufferDP() { return m_CB.GetAddressOf(); }
	CONSTANT_TYPE	GetCBType() { return m_Type; }

	void			SetCBType(CONSTANT_TYPE _type) { m_Type = _type; }

public:
	HRESULT			Create(UINT _elementSize, UINT _elementCount, CONSTANT_TYPE _type);
	void			SetData(void* _data, int _size, int _count); 
	
	void			UpdateData();
	void			UpdateData_CS();

public:
	ConstantBuffer();
	~ConstantBuffer();
};