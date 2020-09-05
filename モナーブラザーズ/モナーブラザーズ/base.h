#pragma once
#include "DxLib.h"

typedef enum
{
	LEFT,
	RIGHT
}DIRE;

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