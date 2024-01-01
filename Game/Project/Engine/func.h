/******************
universal function
******************/

class GameObject;

namespace Object
{
	void Instantiate(GameObject* _target, int _LayerIdx);
	void Destroy(GameObject* _target);

	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	//void DrawDebugCircle();
	//void DrawDebugCube();
	//void DrawDebugSphere();
}

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

	vec.clear();
}

// 한면의 법선 벡터 구하기
//void ComputeNormal(const XMVECTOR& p0, const XMVECTOR& p1, const XMVECTOR& p2, OUT XMVECTOR& out)
//{
//	XMVECTOR u = p1 - p0;
//	XMVECTOR v = p2 - p0;
//
//	out = DirectX::XMVector3Cross(u, v);
//	out = DirectX::XMVector3Normalize(out);
//}