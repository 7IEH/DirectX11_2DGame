#include "pch.h"
#include "EHScriptUI.h"

ScriptUI::ScriptUI(string _scriptName)
	:ComponentUI(_scriptName, "##ScriptUI", COMPONENT_TYPE::SCRIPT)
	, m_ScriptName(_scriptName)
{
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::Render_Update()
{
}