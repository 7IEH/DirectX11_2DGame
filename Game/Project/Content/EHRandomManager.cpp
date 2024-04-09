#include "pch.h"
#include "EHRandomManager.h"

RandomManager::RandomManager()
{

}

RandomManager::~RandomManager()
{

}

void RandomManager::Awake()
{
	m_RandomDevice.seed(static_cast<unsigned int>(time(NULL)));
}

int RandomManager::GenerateNumber(int _st, int _end)
{
	std::uniform_int<int> _unif(_st, _end);
	return _unif(m_RandomDevice);
}