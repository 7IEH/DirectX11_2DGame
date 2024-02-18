#include "pch.h"
#include "EHScriptMgr.h"

#include "EHScript.h"
#include "Scripts.h"

#include "EHPathMgr.h"

ScriptMgr::ScriptMgr()
	:m_Scripts{}
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
		return iter->second;

	return nullptr;
}

void ScriptMgr::Awake()
{
	AddScript<PlayerScript>(L"PlayerScript");
	AddScript<CameraScript>(L"CameraScript");
	AddScript<CameraTargetScript>(L"CameraTargetScript");
	AddScript<TriggerScript>(L"TriggerScript");
	AddScript<OutLineScript>(L"OutLineScript");

	SaveScriptFile();
}

void ScriptMgr::SaveScriptFile()
{
	map<wstring, Script*>::iterator iter = m_Scripts.begin();

	for (;iter != m_Scripts.end();iter++)
	{
		wstring _scriptName = iter->first;
		wstring _absolute = PATH;
		wstring _path = _absolute + L"\\Assets\\Scripts\\" + _scriptName + L".script";

		std::ofstream(_path.c_str());
	}
}