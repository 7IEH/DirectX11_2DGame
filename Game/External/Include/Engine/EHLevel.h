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

public:
	void AddObject(GameObject* _obj, LAYER_TYPE _type, bool _bChildMove = true)
	{
		m_Layers[(UINT)_type]->AddObject(_obj,_bChildMove);
		if (_obj->GetName() == L"")
		{
			wchar_t buf[32] = {};
			swprintf_s(buf, L"GameObject%d", _obj->GetIdx());
			_obj->SetName(buf);
		}		
	}

	Layer* GetLayer(LAYER_TYPE _type) { return m_Layers[(UINT)_type]; }

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();

private:
	void Clear();

public:
	Level();
	virtual ~Level();

	friend class LevelMgr;
};

