#pragma once

#include "EHEntity.h"
#include "EHLayer.h"

#include "EHGameObject.h"

class Layer;
class Level
	:public Entity
{
private:
	Layer* m_Layers[(UINT)LAYER_TYPE::END];

	UINT	m_CollistionMatrix[(UINT)LAYER_TYPE::END];

	UINT	m_CopyCount;

	bool	m_bEditor;
public:
	void AddObject(GameObject* _obj, LAYER_TYPE _type, bool _bChildMove = true)
	{
		m_Layers[(UINT)_type]->AddObject(_obj, _bChildMove);
		if (_obj->GetName() == L"")
		{
			wchar_t buf[32] = {};
			swprintf_s(buf, L"GameObject%d", _obj->GetIdx());
			_obj->SetName(buf);
		}
	}

	Layer* GetLayer(LAYER_TYPE _type) { return m_Layers[(UINT)_type]; }

	GameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(const wstring& _strName, vector<GameObject*>& _vecObj);

	void Load(string _path);

	void AddEmptyObject()
	{
		GameObject* _empty = new GameObject;
		AddObject(_empty, LAYER_TYPE::BACKGROUND1);
	}

	Level* CopyLevel();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();

	CLONE(Level)

private:
	void SetCamera();
	void Clear();

public:
	Level();
	Level(const Level& _origin);
	virtual ~Level();

	friend class LevelMgr;
};

