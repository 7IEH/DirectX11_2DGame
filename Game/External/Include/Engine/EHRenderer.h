#pragma once

#include "EHComponent.h"

#include "EHMesh.h"
#include "EHMaterial.h"

class Renderer
	:public Component
{
private:
	RENDERER_TYPE			m_Type;
	Ptr<Mesh>				m_Mesh;

	Ptr<Material>			m_CurMtrl;
	Ptr<Material>			m_DynamicMtrl;
	Ptr<Material>			m_SharedMtrl;

	bool					m_bIsRender;

public:
	RENDERER_TYPE			GetRenderType() { return m_Type; }
	Ptr<Mesh>				GetMesh() { return m_Mesh; }
	Ptr<Material>			GetMaterial() { return m_CurMtrl; }
	Ptr<Material>			GetDynamicMaterial();
	Ptr<Material>			GetSharedMaterial() { return m_SharedMtrl; }

	bool					GetIsRender() { return m_bIsRender; }
	void					SetIsRender(bool _bIsRender) { m_bIsRender = _bIsRender; }

	void				SetMesh(Ptr<Mesh> _mesh) { m_Mesh = _mesh; }
	void				SetMaterial(Ptr<Material> _material)
	{
		m_SharedMtrl = _material;
		m_CurMtrl = m_SharedMtrl;
		m_DynamicMtrl = nullptr;
	}

public:
	virtual void Render() = 0;
	virtual void Save(std::wofstream* _file) override {};
	virtual void Load(std::wifstream* _file) override {};

	virtual Renderer* Clone() = 0;

public:
	Renderer(RENDERER_TYPE _type);
	Renderer(const Renderer& _origin);
	~Renderer();
};

