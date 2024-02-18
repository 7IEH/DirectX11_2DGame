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

	UINT   m_CollistionMatrix[(UINT)LAYER_TYPE::END];

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
		AddObject(_empty, LAYER_TYPE::BACKGROUND);
	}

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();

	CLONE(Level)

private:
	void Clear();

private:
	void AddTrasnform(GameObject* _obj, Vec4 _position, Vec4 _scale, Vec3 _rotation);
	void AddCamera(GameObject* _obj, PROJECTION_TYPE _proj, CAMERA_TYPE _camtype, UINT _visibleLayer);
	void AddLight2D(GameObject* _obj, LIGHT_TYPE _lighttype, Vec4 _color, Vec4 _ambient, float _angle,float _radius);
	void AddMeshRenderer(GameObject* _obj, wstring _mesh, wstring _material);
	void AddCollider2D(GameObject* _obj, Vec3 _offsetPostion, Vec3 _offsetScale);
	void AddCollider2D(GameObject* _obj, Vec3 _offsetPostion, float _radius);
	void AddAnimator2D(GameObject* _obj, vector<wstring>_aniName);
	void AddScript(GameObject* _obj, vector<wstring>_script);

public:
	Level();
	Level(const Level& _origin);
	virtual ~Level();

	friend class LevelMgr;
};

