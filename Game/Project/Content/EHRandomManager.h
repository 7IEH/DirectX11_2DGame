#pragma once
class RandomManager
	:public Singleton<RandomManager>
{
	Single(RandomManager)
private:
	std::mt19937 m_RandomDevice;

public:
	void Awake();

	int GenerateNumber(int _st, int end);
};

