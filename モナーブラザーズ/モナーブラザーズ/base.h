#pragma once
#include "DxLib.h"

class base
{
public:
	base();
	~base();

	virtual void Update() = 0;
	void Draw();
	VECTOR pos;
	int w, h;
	int speed;

protected:
	int graph;
};