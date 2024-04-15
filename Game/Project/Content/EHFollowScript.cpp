#include "pch.h"
#include "EHFollowScript.h"

#include <EHGameObject.h>

FollowScript::FollowScript()
	:m_pFollowingObj(nullptr)
{
	SetName(L"FollowScript");
}

FollowScript::~FollowScript()
{
}

void FollowScript::Update()
{
	if (nullptr == m_pFollowingObj)
		return;

	Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Transform* _pFollowTr = m_pFollowingObj->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vFollowPos = _pFollowTr->GetRelativePosition();

	assert(_pTr);
	assert(_pFollowTr);

	_vFollowPos += m_vOffSetPos;

	_pTr->SetRelativePosition(_vFollowPos);
}