#include "pch.h"
#include "EHPathMgr.h"

vector<wstring> g_vecName;

int main()
{
	PathMgr::init();
	wstring strProjPath = PathMgr::GetProjectPath();
	wstring strHeaderPath = strProjPath + L"Content\\Scripts.h";
	wstring strCppPath = strProjPath + L"Content\\EHScriptMgr.cpp";

	// 1. 현재 존재하는 모든 스크립트를 알아내야함.
	wstring strScriptIncludePath = PathMgr::GetIncludePath();
	wstring strScriptCode = strScriptIncludePath + L"Content\\";

	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == handle)
		return 0;

	// 예외 리스트 목록을 알아낸다.
	FILE* pExeptList = nullptr;
	_wfopen_s(&pExeptList, L"exeptlist.txt", L"r");

	vector<wstring> strExept;

	if (nullptr != pExeptList)
	{
		wchar_t szName[255] = L"";

		while (true)
		{
			int iLen = fwscanf_s(pExeptList, L"%s", szName, 255);
			if (iLen == -1)
				break;

			strExept.push_back(szName);
		}
		std::fclose(pExeptList);
	}

	while (true)
	{
		// 예외가 아닌경우, 스크립트 이름으로 본다.
		bool bExeption = false;
		for (size_t i = 0; i < strExept.size(); ++i)
		{
			if (!wcscmp(tData.cFileName, strExept[i].c_str()))
			{
				bExeption = true;
				break;
			}
		}

		if (!bExeption)
		{
			if (wstring(tData.cFileName).find(L"Script") != wstring::npos)
			{
				g_vecName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
			}
		}

		if (!FindNextFile(handle, &tData))
			break;
	}

	FindClose(handle);

	// 이전에 CodeGen 이 실행할때 체크해둔 스크립트 목록
	FILE* pScriptListFile = nullptr;
	_wfopen_s(&pScriptListFile, L"ScriptList.txt", L"r");

	if (nullptr != pScriptListFile)
	{
		wchar_t szScriptName[50] = L"";
		vector<wstring> strCurScriptList;
		while (true)
		{
			int iLen = fwscanf_s(pScriptListFile, L"%s", szScriptName, 50);
			if (iLen == -1)
				break;

			strCurScriptList.push_back(szScriptName);
		}
		std::fclose(pScriptListFile);


		if (g_vecName.size() == strCurScriptList.size())
		{
			bool bSame = true;
			for (UINT i = 0; i < g_vecName.size(); ++i)
			{
				if (g_vecName[i] != strCurScriptList[i])
				{
					// 같지 않은게 1개이상 있다
					bSame = false;
					break;
				}
			}

			// 이전 목록과, 현재 스크립트 목록이 완전 일치한다(변경사항 없다)
			if (bSame)
				return 0;
		}
	}

	FILE* pFile = NULL;

	//====================
	// Scripts h 작성
	//====================
	_wfopen_s(&pFile, strHeaderPath.c_str(), L"w");

	fwprintf_s(pFile, L"#pragma once\n");
	fwprintf_s(pFile, L"#include \"Content_enum.h\"\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"#include \"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L".h\"\n");
	}

	std::fclose(pFile);
	//====================
	// ScriptMgr cpp 작성
	//====================
	_wfopen_s(&pFile, strCppPath.c_str(), L"w");

	// 헤더 입력
	fwprintf_s(pFile, L"#include \"pch.h\"\n");
	fwprintf_s(pFile, L"#include \"EHScriptMgr.h\"\n\n");
	fwprintf_s(pFile, L"#include \"EHScript.h\"\n\n");
	fwprintf_s(pFile, L"#include \"Scripts.h\"\n\n");
	fwprintf_s(pFile, L"#include \"EHPathMgr.h\"\n\n");

	// 생성자
	fwprintf_s(pFile, L"\nScriptMgr::ScriptMgr()\n\t: m_Scripts{}\n{\n}\n\n");

	// 소멸자
	fwprintf_s(pFile, L"\nScriptMgr::~ScriptMgr()\n{\n\tReleaseMap(m_Scripts);\n}\n\n");

	// 첫번째 함수 작성
	fwprintf_s(pFile, L"\nScript* ScriptMgr::FindScript(wstring _name)\n{\n");

	fwprintf_s(pFile, L"\tmap<wstring, Script*>::iterator iter = m_Scripts.find(_name);\n");

	fwprintf_s(pFile, L"\tif (m_Scripts.end() != iter)\n");
	fwprintf_s(pFile, L"\t  return iter->second;");

	fwprintf_s(pFile, L"\treturn nullptr;\n");
	fwprintf_s(pFile, L"}\n\n");

	// 두번째 함수
	fwprintf_s(pFile, L"\nvoid ScriptMgr::Awake()\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tAddScript<");
		fwprintf_s(pFile, g_vecName[i].substr(2).c_str());
		fwprintf_s(pFile, L">(L\"");
		fwprintf_s(pFile, g_vecName[i].substr(2).c_str());
		fwprintf_s(pFile, L"\");\n");
	}

	fwprintf_s(pFile, L"\tSaveScriptFile();\n");

	fwprintf_s(pFile, L"}\n\n");


	// 세번째 함수
	fwprintf_s(pFile, L"\nvoid ScriptMgr::SaveScriptFile()\n{\n");

	fwprintf_s(pFile, L"\tmap<wstring, Script*>::iterator iter = m_Scripts.begin();\n");
	fwprintf_s(pFile, L"\tfor(;iter != m_Scripts.end(); iter++)\n\t{\n");
	fwprintf_s(pFile, L"\t\twstring _scriptName = iter->first;\n");
	fwprintf_s(pFile, L"\t\twstring _absoulte = PATH;\n");
	fwprintf_s(pFile, L"\t\twstring _path = _absoulte + L\"\\\\Assets\\\\Scripts\\\\\" + _scriptName + L\".script\";\n");
	fwprintf_s(pFile, L"\t\tstd::ofstream(_path.c_str());\n");

	fwprintf_s(pFile, L"\t}\n");
	fwprintf_s(pFile, L"}\n\n");

	std::fclose(pFile);


	// 다음번 실행시 비교하기위한 정보 저장
	_wfopen_s(&pFile, L"ScriptList.txt", L"w");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\n");
	}

	std::fclose(pFile);

	return 0;
}