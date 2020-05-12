#pragma once
#include "base.h"

class player :public base
{
public:
	player();
	virtual ~player();

	void Draw();
	void Update();

	int pGraph[3];
};