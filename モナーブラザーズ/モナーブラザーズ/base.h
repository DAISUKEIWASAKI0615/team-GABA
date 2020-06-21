#pragma once
#include "DxLib.h"

class base
{
public:
	base();
	virtual ~base();

	virtual void Update() = 0;
	void Draw();
	VECTOR pos;
	int w, h;
	float speed;

protected:
	int graph;
};