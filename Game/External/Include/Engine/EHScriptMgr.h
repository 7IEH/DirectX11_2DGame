#pragma once

class Script;
class ScriptMgr
	:public Singleton<ScriptMgr>
{
	Single(ScriptMgr)
private:
	map<wstring, Script*> m_Scripts;

public:
	Script* FindScript(wstring _name);

	template<typename T>
	T* AddScript(wstring _name)
	{
		Script* _script = FindScript(_name);

		if (nullptr != _script)
			return dynamic_cast<T*>(_script);

		_script = new T;

		m_Scripts.insert(make_pair(_name, _script));

		return dynamic_cast<T*>(_script);
	}

public:
	map<wstring, Script*>& GetScripts() { return m_Scripts; }

private:
	void SaveScriptFile();

public:
	void Awake();
};

