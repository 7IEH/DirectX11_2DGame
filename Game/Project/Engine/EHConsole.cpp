#include "pch.h"
#include "EHConsole.h"

#include "EHDebugMgr.h"
#include "EHTimeMgr.h"

Console::Console()
	:UI("Console","##Console")
{
}

Console::~Console()
{
}

void Console::Render_Update()
{
	vector<string> _msg = DebugMgr::GetInst()->GetDebugMessage();

	for (int i = 0;i < _msg.size();i++)
	{
		m_ConsoleMessage.push_back(_msg[i]);
	}

	static size_t _line = 0;
	m_AcctimeforDebug += DT;
	if (m_AcctimeforDebug >= m_OutputTime)
	{
		_line++;

		if (_line >= m_ConsoleMessage.size())
		{
			_line = m_ConsoleMessage.size();
		}

		m_AcctimeforDebug = 0.f;
	}

	ImGui::BeginChild("Scrolling");
	for (size_t i = 0;i < _line;i++)
	{
		ImGui::Text(m_ConsoleMessage[i].c_str());
	}
	ImGui::EndChild();
}