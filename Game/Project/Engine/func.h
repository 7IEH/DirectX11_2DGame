/******************
universal function
******************/
#include "pch.h"

// 메모리 해제 ( 배열용 )
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

// 메모리 해제 ( 벡터용 )
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
}

// 한면의 법선 벡터 구하기
void ComputeNomral(const XMVECTOR& p0, const XMVECTOR& p1, const XMVECTOR& p2, OUT XMVECTOR& out)
{
	XMVECTOR u = p1 - p0;
	XMVECTOR v = p2 - p0;

	out = DirectX::XMVector3Cross(u, v);
	out = DirectX::XMVector3Normalize(out);
}