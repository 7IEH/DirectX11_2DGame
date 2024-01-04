#include "pch.h"
#include "EHCollisionMgr.h"

#include "EHLevelMgr.h"
#include "EHGameObject.h"

#include "EHCollider.h"
#include "EHCollider2D.h"
#include "EHCircleCollider2D.h"

CollisionMgr::CollisionMgr()
	:m_CollisionMatrix{}
	, m_MemoryOfCollision{}
{
}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::LayerCheck(LAYER_TYPE _layer1, LAYER_TYPE _layer2)
{
	UINT _col = (UINT)_layer1;
	UINT _row = (UINT)_layer2;

	if (_row > _col)
	{
		int temp = _row;
		_row = _col;
		_col = temp;
	}

	m_CollisionMatrix[_row] |= (1 << _col);
}

void CollisionMgr::Update()
{
	for (UINT row = 0;row < (UINT)LAYER_TYPE::END;row++)
	{
		for (UINT col = row; col < (UINT)LAYER_TYPE::END;col++)
		{
			if (!(m_CollisionMatrix[row] & (1 << col)))
				continue;

			CollisionLayer(row, col);
		}
	}
}

void CollisionMgr::CollisionLayer(UINT _left, UINT _right)
{
	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	Layer* _leftLayer = _curLevel->GetLayer(LAYER_TYPE(_left));
	Layer* _rightLayer = _curLevel->GetLayer(LAYER_TYPE(_right));

	const vector<GameObject*>& _leftObj = _leftLayer->GetLayerObject();
	const vector<GameObject*>& _rightObj = _rightLayer->GetLayerObject();

	for (size_t i = 0; i < _leftObj.size();i++)
	{		
		if (nullptr == _leftObj[i]->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D))
			continue;

		size_t j = 0;

		if (_left == _right)
		{
			j = j + 1;
		}

		for (;j < _rightObj.size();j++)
		{
			if (nullptr == _rightObj[i]->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D))
				continue;

			CollisionID id = {};
			id._left = _leftObj[i]->GetID();
			id._right = _rightObj[j]->GetID();

			map<UINT_PTR, bool>::iterator iter = m_MemoryOfCollision.find(id.ID);

			if (iter == m_MemoryOfCollision.end())
			{
				m_MemoryOfCollision.insert({ id.ID,false });
				iter = m_MemoryOfCollision.find(id.ID);
			}

			bool bDead = _leftObj[i]->GetDead() || _rightObj[i]->GetDead();

			Collider* _leftCol = _leftObj[i]->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D);
			Collider* _rightCol = _rightObj[i]->GetComponent<Collider>(COMPONENT_TYPE::COLLIDER2D);

			if (CollisionCollider(_leftCol, _rightCol))
			{
				if (iter->second)
				{
					if (bDead)
					{
						_leftCol->OnTriggerExit(_rightCol);
						_rightCol->OnTriggerExit(_leftCol);
						iter->second = FALSE;
					}
					else
					{
						_leftCol->OnTriggerStay(_rightCol);
						_rightCol->OnTriggerStay(_leftCol);
					}
				}
				else
				{
					if (bDead)
					{
						_leftCol->OnTriggerExit(_rightCol);
						_rightCol->OnTriggerExit(_leftCol);
					}
					else
					{
						_leftCol->OnTriggerEnter(_rightCol);
						_rightCol->OnTriggerEnter(_leftCol);
						iter->second = TRUE;
					}
				}
			}
			else
			{
				if (iter->second)
				{
					_leftCol->OnTriggerExit(_rightCol);
					_rightCol->OnTriggerExit(_leftCol);
					iter->second = FALSE;
				}
			}
		}
	}
}

bool CollisionMgr::CollisionCollider(Collider* _colLeft, Collider* _colRight)
{
	COLLIDER_TYPE _leftType = _colLeft->GetColliderType();
	COLLIDER_TYPE _rightType = _colRight->GetColliderType();


	if (_leftType == COLLIDER_TYPE::BOXCOLLIDER2D && _rightType == COLLIDER_TYPE::BOXCOLLIDER2D)
	{
		return Box2DCollisionCheck(_colLeft, _colRight);
	}
	else if (_leftType == COLLIDER_TYPE::CIRCLECOLLDIER2D && _rightType == COLLIDER_TYPE::CIRCLECOLLDIER2D)
	{
		return Circle2DCollisionCheck(_colLeft, _colRight);
	}
	else if (_leftType == COLLIDER_TYPE::BOXCOLLIDER && _rightType == COLLIDER_TYPE::BOXCOLLIDER)
	{
		return Box3DCollisionCheck(_colLeft, _colRight);
	}
	else if (_leftType == COLLIDER_TYPE::SPHERECOLLDIER && _rightType == COLLIDER_TYPE::SPHERECOLLDIER)
	{
		return SphereCollisionCheck(_colLeft, _colRight);
	}

	return FALSE;
}

bool CollisionMgr::Box2DCollisionCheck(Collider* _colLeft, Collider* _colRight)
{
	Collider2D* _box2DLeft = dynamic_cast<Collider2D*>(_colLeft);
	Collider2D*	_box2DRight = dynamic_cast<Collider2D*>(_colRight);

	if (nullptr == _box2DLeft || nullptr == _box2DRight)
		return FALSE;

	Matrix _leftMat = _box2DLeft->GetOffsetMatrix();
	Matrix _rightMat = _box2DRight->GetOffsetMatrix();

	// 가상의 점들
	static Vec3	_Points[4] = { {-0.5f,0.5f,0.f},{0.5f,0.5f,0.f},
							{0.5f,-0.5f,0.f},{-0.5f,-0.5f,0.f} };

	Vec3 _Proj[4] = {};

	_Proj[0] = XMVector3TransformCoord(_Points[1], _leftMat) - XMVector3TransformCoord(_Points[0], _leftMat);
	_Proj[1] = XMVector3TransformCoord(_Points[3], _leftMat) - XMVector3TransformCoord(_Points[0], _leftMat);
	_Proj[2] = XMVector3TransformCoord(_Points[1], _rightMat) - XMVector3TransformCoord(_Points[0], _rightMat);
	_Proj[3] = XMVector3TransformCoord(_Points[3], _rightMat) - XMVector3TransformCoord(_Points[0], _rightMat);

	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _rightMat) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _leftMat);

	for (int i = 0;i < 4;i++)
	{
		float _test = 0.f;
		Vec3 _normalize = _Proj[i];
		_normalize.Normalize();
		for (int j = 0;j < 4;j++)
		{
			_test += abs(_normalize.Dot(_Proj[j]));
		}
		_test /= 2.f;
		float _compare = abs(_normalize.Dot(vCenter));

		if (_test < _compare)
		{
			return FALSE;
		}
	}

	return TRUE;
}

bool CollisionMgr::Circle2DCollisionCheck(Collider* _colLeft, Collider* _colRight)
{
	CircleCollider2D* _circle2DLeft = dynamic_cast<CircleCollider2D*>(_colLeft);
	CircleCollider2D* _circle2DRight = dynamic_cast<CircleCollider2D*>(_colRight);

	if (nullptr == _circle2DLeft || nullptr == _circle2DRight)
		return FALSE;

	float _leftRadius = _circle2DLeft->GetRadius();
	float _rightRadius = _circle2DRight->GetRadius();

	const Matrix _leftMat = _circle2DLeft->GetOffsetMat();
	const Matrix _rightMat = _circle2DRight->GetOffsetMat();

	Vec3 _CenterVec3 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _leftMat) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _rightMat);
	float _CenterDistance = _CenterVec3.Length();
	float _radiusCombine = _leftRadius + _rightRadius;
	_radiusCombine /= 2.f;

	if (_CenterDistance > _radiusCombine)
	{
		return FALSE;
	}

	return TRUE;
}

bool CollisionMgr::Box3DCollisionCheck(Collider* _colLeft, Collider* _colRight)
{
	return TRUE;
}

bool CollisionMgr::SphereCollisionCheck(Collider* _colLeft, Collider* _colRight)
{
	return TRUE;
}