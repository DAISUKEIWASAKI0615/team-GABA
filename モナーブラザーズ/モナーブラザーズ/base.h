#pragma once
#include "DxLib.h"
#include "main.h"

class base
{
public:
	base();
	~base();

	virtual void Update() = 0;
	void Draw();
	VECTOR pos;
	int w, h;
	float speed;

protected:
	int graph;
};