#include "pch.h"
#include "EHProjecTileScript.h"

#include <EHGameObject.h>
#include <EHLevelMgr.h>
#include <EHTimeMgr.h>

ProjecTileScript::ProjecTileScript()
	:m_fSpeed(0.f)
	, m_eDir(Dir::UP)
	, m_bNotDir(FALSE)
{
	SetName(L"ProjecTileScript");
}

ProjecTileScript::~ProjecTileScript()
{
}

void ProjecTileScript::Update()
{
	if (m_bNotDir)
		return;

	Transform* _pTr = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	Vec4 _vPos = _pTr->GetRelativePosition();

	switch (m_eDir)
	{
	case Dir::UP:
	{
		_vPos.y += m_fSpeed * DT;
	}
	break;
	case Dir::DOWN:
	{
		_vPos.y -= m_fSpeed * DT;
	}
	break;
	case Dir::LEFT:
	{
		_vPos.x -= m_fSpeed * DT;
		_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 90.f));
	}
	break;
	case Dir::RIGHT:
	{
		_vPos.x += m_fSpeed * DT;
		_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 90.f));
	}
	break;
	default:
		break;
	}

	_pTr->SetRelativePosition(_vPos);
}

void ProjecTileScript::OnTriggerEnter(Collider* _other)
{
	LAYER_TYPE _eType = _other->GetOwner()->GetLayerType();

	if (LAYER_TYPE::PLAYER == _eType)
	{
		Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
		LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<PlayerScript>(L"PlayerScript")->GetPlayerPref()->_iCurHp -= m_iDamage;


		if (m_bNotDir)
			return;

		_pAnim->Play(m_sDeadAnimName, FALSE);
		GetOwner()->SetDead(TRUE);
	}

	if (LAYER_TYPE::STRUCTURE_OBSTACLE_ROCK == _eType || LAYER_TYPE::STRUCTURE_WALL_BOTTOM == _eType ||
		LAYER_TYPE::STRUCTURE_WALL_LEFT == _eType || LAYER_TYPE::STRUCTURE_WALL_RIGHT == _eType ||
		LAYER_TYPE::STRUCTURE_WALL_TOP == _eType)
	{
		Animator2D* _pAnim = GetOwner()->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);
		_pAnim->Play(m_sDeadAnimName, FALSE);
		GetOwner()->SetDead(TRUE);
	}
}