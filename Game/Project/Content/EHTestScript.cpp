#include "pch.h"
#include "EHTestScript.h"

#include "EHGameObject.h"
#include "EHTimeMgr.h"

TestScript::TestScript()
	:m_AccTime(0.f)
{
}

TestScript::~TestScript()
{
}

void TestScript::Update()
{
	MeshRenderer* _mr =  GetOwner()->GetComponent<MeshRenderer>(COMPONENT_TYPE::RENDERER);
	Ptr<Material> _mat = _mr->GetMaterial();
	m_AccTime += DT;
	_mat->SetMaterialParam(FLOAT_0, m_AccTime);

	if (m_AccTime >= 5.f)
	{
		GetOwner()->SetDead(TRUE);
	}
}