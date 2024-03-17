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
	// 동적 재질
	if (_origin.m_SharedMtrl != _origin.m_CurMtrl)
	{
		assert(_origin.m_DynamicMtrl.Get());

		// 복사 렌더 컴포넌트도 별도의 동적재질을 생성한다.
		GetDynamicMaterial();

		// 원본 렌더컴포넌트의 동적재질 값을 현재 생성한 동적재질로 복사한다.
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
	// 이미 동적재질을 보유하고 있으면 그걸 준다.
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	// 공유재질이 있다면
	if (nullptr != m_SharedMtrl)
	{
		// 공유재질을 복사해서 동적재질을 만들고 그걸 현재 사용재질로 설정한다.
		m_DynamicMtrl = m_SharedMtrl->Clone();
		m_CurMtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}

	return nullptr;
}