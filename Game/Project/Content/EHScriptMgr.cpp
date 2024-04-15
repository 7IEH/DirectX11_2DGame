#include "pch.h"
#include "EHScriptMgr.h"

#include "EHScript.h"

#include "Scripts.h"

#include "EHPathMgr.h"


ScriptMgr::ScriptMgr()
	: m_Scripts{}
{
}


ScriptMgr::~ScriptMgr()
{
	ReleaseMap(m_Scripts);
}


Script* ScriptMgr::FindScript(wstring _name)
{
	map<wstring, Script*>::iterator iter = m_Scripts.find(_name);
	if (m_Scripts.end() != iter)
	  return iter->second;	return nullptr;
}


void ScriptMgr::Awake()
{
	AddScript<BabySlimeScript>(L"BabySlimeScript");
	AddScript<BossUIScript>(L"BossUIScript");
	AddScript<CameraScript>(L"CameraScript");
	AddScript<CameraTargetScript>(L"CameraTargetScript");
	AddScript<DungeonEntranceScript>(L"DungeonEntranceScript");
	AddScript<DungeonScript>(L"DungeonScript");
	AddScript<EnemyScript>(L"EnemyScript");
	AddScript<FollowScript>(L"FollowScript");
	AddScript<GolemBossSceneScript>(L"GolemBossSceneScript");
	AddScript<GolemButlerScript>(L"GolemButlerScript");
	AddScript<GolemDungeonScript>(L"GolemDungeonScript");
	AddScript<GolemKingATK1Script>(L"GolemKingATK1Script");
	AddScript<GolemKingATK2Script>(L"GolemKingATK2Script");
	AddScript<GolemKingATK3Script>(L"GolemKingATK3Script");
	AddScript<GolemKingATK4Script>(L"GolemKingATK4Script");
	AddScript<GolemKingScript>(L"GolemKingScript");
	AddScript<GolemSoilderScript>(L"GolemSoilderScript");
	AddScript<GolemTurretScript>(L"GolemTurretScript");
	AddScript<GUI_LineScript>(L"GUI_LineScript");
	AddScript<ItemScript>(L"ItemScript");
	AddScript<Light2DScript>(L"Light2DScript");
	AddScript<LoadingSceneScript>(L"LoadingSceneScript");
	AddScript<MeleeEnemyScript>(L"MeleeEnemyScript");
	AddScript<OutLineScript>(L"OutLineScript");
	AddScript<PlayerScript>(L"PlayerScript");
	AddScript<PortionScript>(L"PortionScript");
	AddScript<ProjecTileScript>(L"ProjecTileScript");
	AddScript<PUIScript>(L"PUIScript");
	AddScript<RangedEnemyScript>(L"RangedEnemyScript");
	AddScript<SlimeHermitScript>(L"SlimeHermitScript");
	AddScript<SlotScript>(L"SlotScript");
	AddScript<SpawnEffectScript>(L"SpawnEffectScript");
	AddScript<SpawnRockScript>(L"SpawnRockScript");
	AddScript<TangleScript>(L"TangleScript");
	AddScript<TestScript>(L"TestScript");
	AddScript<TitleScript>(L"TitleScript");
	AddScript<TownScript>(L"TownScript");
	AddScript<TriggerScript>(L"TriggerScript");
	AddScript<TutorialScript>(L"TutorialScript");
	SaveScriptFile();
}


void ScriptMgr::SaveScriptFile()
{
	map<wstring, Script*>::iterator iter = m_Scripts.begin();
	for(;iter != m_Scripts.end(); iter++)
	{
		wstring _scriptName = iter->first;
		wstring _absoulte = PATH;
		wstring _path = _absoulte + L"\\Assets\\Scripts\\" + _scriptName + L".script";
		std::ofstream(_path.c_str());
	}
}

