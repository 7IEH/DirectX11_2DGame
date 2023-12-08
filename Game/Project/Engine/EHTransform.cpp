#include "pch.h"
#include "EHTransform.h"

#include "EHDevice.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	,m_Trasnform{}
{
	m_Trasnform = new tTransform();
	m_Trasnform->_Position = {};
	m_Trasnform->_Scale = {1.f,1.f,1.f,1.f};
}

Transform::~Transform()
{
}

void Transform::Create()
{
	CreateConstantBuffer();
}

void Transform::Tick()
{
	D3D11_MAPPED_SUBRESOURCE tSubData = {};
	::memset(&tSubData, 0, sizeof(tSubData));
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubData);
	::memcpy(tSubData.pData, m_Trasnform, sizeof(tTransform));
	CONTEXT->Unmap(m_CB.Get(), 0);
	CONTEXT->VSSetConstantBuffers(0, 1, m_CB.GetAddressOf());
}

void Transform::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC tDesc = {};
	::memset(&tDesc, 0, sizeof(tDesc));
	tDesc.ByteWidth = sizeof(tTransform);
	tDesc.StructureByteStride = sizeof(tTransform);

	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	DEVICE->CreateBuffer(&tDesc, nullptr, m_CB.GetAddressOf());
}
