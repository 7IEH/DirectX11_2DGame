#pragma once

#include "EHThreadTask.h"

class MapGenerator
	:public ThreadTask
{
public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	void Divide();

public:
	MapGenerator();
	virtual ~MapGenerator();
};

