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
	AddScript<CameraScript>(L"CameraScript");
	AddScript<CameraTargetScript>(L"CameraTargetScript");
	AddScript<DungeonScript>(L"DungeonScript");
	AddScript<GUI_LineScript>(L"GUI_LineScript");
	AddScript<Light2DScript>(L"Light2DScript");
	AddScript<OutLineScript>(L"OutLineScript");
	AddScript<PlayerScript>(L"PlayerScript");
	AddScript<SlotScript>(L"SlotScript");
	AddScript<TestScript>(L"TestScript");
	AddScript<TitleScript>(L"TitleScript");
	AddScript<TriggerScript>(L"TriggerScript");
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

