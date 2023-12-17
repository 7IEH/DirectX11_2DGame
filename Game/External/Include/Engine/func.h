
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

template <typename T>
void ReleaseVector(vector<T*>&vec)
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