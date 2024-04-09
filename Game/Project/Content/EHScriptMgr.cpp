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
	AddScript<CameraScript>(L"CameraScript");
	AddScript<CameraTargetScript>(L"CameraTargetScript");
	AddScript<DungeonEntranceScript>(L"DungeonEntranceScript");
	AddScript<DungeonScript>(L"DungeonScript");
	AddScript<EnemyScript>(L"EnemyScript");
	AddScript<GolemBossSceneScript>(L"GolemBossSceneScript");
	AddScript<GolemButlerScript>(L"GolemButlerScript");
	AddScript<GolemDungeonScript>(L"GolemDungeonScript");
	AddScript<GolemTurretScript>(L"GolemTurretScript");
	AddScript<GUI_LineScript>(L"GUI_LineScript");
	AddScript<ItemScript>(L"ItemScript");
	AddScript<Light2DScript>(L"Light2DScript");
	AddScript<MeleeEnemyScript>(L"MeleeEnemyScript");
	AddScript<OutLineScript>(L"OutLineScript");
	AddScript<PlayerScript>(L"PlayerScript");
	AddScript<PortionScript>(L"PortionScript");
	AddScript<ProjecTileScript>(L"ProjecTileScript");
	AddScript<PUIScript>(L"PUIScript");
	AddScript<RangedEnemyScript>(L"RangedEnemyScript");
	AddScript<SlotScript>(L"SlotScript");
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

