#include "pch.h"
#include "EHMeleeEnemyScript.h"

MeleeEnemyScript::MeleeEnemyScript()
{
	SetName(L"MeleeEnemyScript");
	SetState(EnemyState::Idle);
	SetType(EnemyType::Melee);
}

MeleeEnemyScript::~MeleeEnemyScript()
{
}

void MeleeEnemyScript::Update()	
{
	EnemyScript::Update();
}

void MeleeEnemyScript::Idle()
{
}

void MeleeEnemyScript::Chase()
{
}

void MeleeEnemyScript::Attack()
{
}