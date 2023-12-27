/******************
universal function
******************/

class GameObject;

namespace Object
{
	void Instantiate(GameObject* _target, int _LayerIdx);
	void Destroy(GameObject* _target);
}

// �޸� ���� ( �迭�� )
template <typename T, int SIZE>
void ReleaseArray(T* (&arr)[SIZE])
{
	for (int _idx = 0;_idx < SIZE;_idx++)
	{
		if (arr[_idx] != nullptr)
		{
			delete arr[_idx];
			arr[_idx] = nullptr;
		}
	}
}

// �޸� ���� ( ���Ϳ� )
template <typename T>
void ReleaseVector(vector<T*>& vec)
{
	for (size_t _idx = 0;_idx < vec.size();_idx++)
	{
		if (vec[_idx] != nullptr)
		{
			delete vec[_idx];
			vec[_idx] = nullptr;
		}
	}

	vec.clear();
}

// �Ѹ��� ���� ���� ���ϱ�
//void ComputeNormal(const XMVECTOR& p0, const XMVECTOR& p1, const XMVECTOR& p2, OUT XMVECTOR& out)
//{
//	XMVECTOR u = p1 - p0;
//	XMVECTOR v = p2 - p0;
//
//	out = DirectX::XMVector3Cross(u, v);
//	out = DirectX::XMVector3Normalize(out);
//}