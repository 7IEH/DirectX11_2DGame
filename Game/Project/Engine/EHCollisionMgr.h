#pragma once

class Collider2D;

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
	void LayerCheck(LAYER_TYPE _layer1, LAYER_TYPE _layer2);
	void Clear()
	{
		for (UINT i = 0; i < (UINT)LAYER_TYPE::END;i++)
		{
			m_CollisionMatrix[i] = 0;
		}
	}

public:
	void Update();

private:
	void CollisionLayer(UINT _left, UINT _right);
	bool CollisionCollider(Collider2D* _pLeft, Collider2D* _pRight);
};

