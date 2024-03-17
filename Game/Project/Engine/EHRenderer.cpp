#include "pch.h"
#include "EHRenderer.h"

Renderer::Renderer(RENDERER_TYPE _type)
	:Component(COMPONENT_TYPE::RENDERER)
	, m_Type(_type)
	, m_Mesh(nullptr)
	, m_CurMtrl(nullptr)
	, m_DynamicMtrl(nullptr)
	, m_SharedMtrl(nullptr)
{
}

Renderer::Renderer(const Renderer& _origin)
	:Component(_origin)
	, m_Type(_origin.m_Type)
	, m_Mesh(_origin.m_Mesh)
	, m_SharedMtrl(_origin.m_SharedMtrl)
{
	// ���� ����
	if (_origin.m_SharedMtrl != _origin.m_CurMtrl)
	{
		assert(_origin.m_DynamicMtrl.Get());

		// ���� ���� ������Ʈ�� ������ ���������� �����Ѵ�.
		GetDynamicMaterial();

		// ���� ����������Ʈ�� �������� ���� ���� ������ ���������� �����Ѵ�.
		*m_DynamicMtrl.Get() = *_origin.m_DynamicMtrl.Get();
	}
	else
	{
		m_CurMtrl = m_SharedMtrl;
	}
}

Renderer::~Renderer()
{
}

Ptr<Material> Renderer::GetDynamicMaterial()
{
	// �̹� ���������� �����ϰ� ������ �װ� �ش�.
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	// ���������� �ִٸ�
	if (nullptr != m_SharedMtrl)
	{
		// ���������� �����ؼ� ���������� ����� �װ� ���� ��������� �����Ѵ�.
		m_DynamicMtrl = m_SharedMtrl->Clone();
		m_CurMtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}

	return nullptr;
}