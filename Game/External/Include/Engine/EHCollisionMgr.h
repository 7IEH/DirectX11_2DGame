#pragma once

class Collider;

union CollisionID
{
	struct
	{
		UINT _left;
		UINT _right;
	};
	UINT_PTR ID;
};


class CollisionMgr
	:public Singleton<CollisionMgr>
{
	Single(CollisionMgr)
private:
	UINT					m_CollisionMatrix[(UINT)LAYER_TYPE::END];
	map<UINT_PTR, bool>		m_MemoryOfCollision;

public:
	void LayerCheck(LAYER_TYPE _layer1, LAYER_TYPE _layer2, bool _flag = TRUE);
	void Clear()
	{
		for (UINT i = 0; i < (UINT)LAYER_TYPE::END;i++)
		{
			m_CollisionMatrix[i] = 0;
		}
	}


	UINT* GetCollisionMatrix() { return m_CollisionMatrix; }
	void  SetCollisionMatrix(UINT _collisionMatrix[(UINT)LAYER_TYPE::END])
	{
		for (int i = 0;i < (UINT)LAYER_TYPE::END;i++)
		{
			m_CollisionMatrix[i] = _collisionMatrix[i];
		}
	}

public:
	void Update();

private:
	void CollisionLayer(UINT _left, UINT _right);
	bool CollisionCollider(Collider* _colLeft, Collider* _colRight);

private:
	// 2D
	bool Box2DCollisionCheck(Collider* _colLeft, Collider* _colRight);
	bool Circle2DCollisionCheck(Collider* _colLeft, Collider* _colRight);

	// 3D
	bool Box3DCollisionCheck(Collider* _colLeft, Collider* _colRight);
	bool SphereCollisionCheck(Collider* _colLeft, Collider* _colRight);
};

