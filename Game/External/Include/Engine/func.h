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
	void DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	//void DrawDebugCube();
	//void DrawDebugSphere();
}

namespace SceneManager
{
	void LoadScene(const wstring& _sceneName);
}

namespace EH
{
	string ConvertString(wstring& _wstr);

	wstring ConvertWstring(string& _str);

	string ConvertString(const std::filesystem::path::string_type _str);

	void InputVector2(string _line, OUT Vec2& _output);

	// �޸��忡�� ������ ���� ��ȯ
	void InputVector3(string _line, OUT Vec3& _output);

	// �޸��忡�� ������ ���� ��ȯ
	void InputVector3(string _line, OUT Vec4& _output);

	// �޸��忡�� ������ ���� ��ȯ
	void InputVector4(string _line, OUT Vec4& _output);

	// �޸��忡 �� ���� ���� ��ȯ

	string WriteVector2(Vec2 _data);

	string WriteVector3(Vec3 _data);

	string WriteVector4(Vec4 _data);
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

// �޸� ���� ( Map�� ) 
template<typename T, typename Y>
void ReleaseMap(map<T, Y>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
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