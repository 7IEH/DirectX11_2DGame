#pragma once
class ConstantBuffer
{
private:
	ComPtr<ID3D11Buffer>	m_CB;
	CONSTANT_TYPE			m_Type;

public:
	ID3D11Buffer*	GetBuffer() { return m_CB.Get(); }
	ID3D11Buffer**	GetBufferDP() { return m_CB.GetAddressOf(); }
	CONSTANT_TYPE	GetCBType() { return m_Type; }

public:
	void			SetData(void* _data, int _size, int _count); 
	void			UpdateData();

public:
	ConstantBuffer();
	virtual ~ConstantBuffer();
};