#pragma once

class Entity;

class GarbageCollector
	:public Singleton<GarbageCollector>
{
	Single(GarbageCollector)
private:
	vector<Entity*> m_ExitObject;

public:
	void AddExitObject(Entity* _entity) { m_ExitObject.push_back(_entity); }

	void Update();
};

